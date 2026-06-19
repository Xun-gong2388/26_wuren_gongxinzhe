#!/usr/bin/env python3
"""
sim_bringup.launch.py  (Gazebo Harmonic 版本)
----------------------------------------------
完整仿真启动文件:
  1. 设置 GZ_SIM_RESOURCE_PATH (加载锥桶等赛道模型)
  2. 处理 xacro -> URDF -> robot_description
  3. 启动 Gazebo Harmonic 并加载 map.world
  4. 启动 robot_state_publisher (发布TF)
  5. 启动 ros_gz_bridge (Gazebo传感器 -> ROS2话题)
  6. (可选) 启动 sim_perception 感知节点

运行方式:
  ros2 launch car_sim sim_bringup.launch.py

  # 同时启动感知节点:
  ros2 launch car_sim sim_bringup.launch.py with_perception:=true
"""

import os
import sys

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch.actions import (
    DeclareLaunchArgument,
    ExecuteProcess,
    SetEnvironmentVariable,
)
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():

    # ============================================================
    # 路径计算
    # ============================================================
    try:
        car_sim_share = get_package_share_directory('car_sim')
    except Exception:
        car_sim_share = os.path.dirname(os.path.dirname(__file__))
        print(f'[WARN] 无法通过ament找到car_sim, 回退到: {car_sim_share}')

    # car_sim 源码目录
    car_sim_src = os.path.join(car_sim_share, '..', '..', '..', 'src', 'car_sim')
    car_sim_src = os.path.normpath(car_sim_src)
    if not os.path.isdir(car_sim_src):
        car_sim_src = car_sim_share  # 回退

    tracks_dir = os.path.normpath(os.path.join(car_sim_src, '..', 'tracks'))
    car_sim_urdf = os.path.join(car_sim_src, 'urdf')

    print(f'[INFO] car_sim 源码目录: {car_sim_src}')
    print(f'[INFO] tracks 目录: {tracks_dir}')
    print(f'[INFO] urdf 目录: {car_sim_urdf}')

    # ============================================================
    # Launch 参数
    # ============================================================
    with_perception = LaunchConfiguration('with_perception', default='false')

    declare_with_perception = DeclareLaunchArgument(
        'with_perception', default_value='false',
        description='是否同时启动 sim_perception 感知节点')

    # ============================================================
    # GZ_SIM_RESOURCE_PATH (Harmonic 的模型路径)
    #   让 Gazebo 找到 blue_cone / yellow_cone / shixi 等模型
    # ============================================================
    gazebo_model_paths = [
        os.path.join(tracks_dir, 'models'),
        os.path.join(os.path.expanduser('~'), '.gazebo', 'models'),
        '/usr/share/gazebo/models',
        '/usr/share/gz/gz-sim',
    ]
    # 从现有环境变量中追加
    existing_gz_path = os.environ.get('GZ_SIM_RESOURCE_PATH', '')
    if existing_gz_path:
        gazebo_model_paths.insert(0, existing_gz_path)

    gz_resource_path_str = ':'.join(
        p for p in gazebo_model_paths if os.path.isdir(p)
    )
    print(f'[INFO] GZ_SIM_RESOURCE_PATH = {gz_resource_path_str}')

    set_gz_resource_path = SetEnvironmentVariable(
        name='GZ_SIM_RESOURCE_PATH',
        value=gz_resource_path_str
    )

    # ============================================================
    # Xacro -> URDF (供 robot_state_publisher 发布TF)
    # ============================================================
    import xacro

    test_xacro_path = os.path.join(car_sim_urdf, 'test.xacro')
    orig_xacro_path = os.path.join(car_sim_urdf, 'car_sim.xacro')

    if os.path.isfile(test_xacro_path):
        xacro_file = test_xacro_path
        print(f'[INFO] 使用 xacro 文件: {xacro_file}')
    else:
        xacro_file = orig_xacro_path
        print(f'[WARN] test.xacro 不存在, 回退到: {xacro_file}')

    doc = xacro.process_file(xacro_file)
    robot_description = doc.toxml()
    print(f'[INFO] URDF 生成成功, 长度: {len(robot_description)} 字符')

    # 将 URDF 写入文件，供 Gazebo 通过 map.world 的 <include> 加载
    urdf_output_path = os.path.join(car_sim_urdf, 'car_sim.urdf')
    with open(urdf_output_path, 'w', encoding='utf-8') as f:
        f.write(robot_description)
    print(f'[INFO] URDF 已写入: {urdf_output_path}')

    # ============================================================
    # 查找 world 文件
    # ============================================================
    world_path = os.path.join(tracks_dir, 'map.world')
    if not os.path.isfile(world_path):
        print(f'[ERROR] 未找到 map.world 于 {world_path}')
        sys.exit(1)

    # ============================================================
    # Gazebo Harmonic (gz sim)
    #   -r 表示启动即开始仿真, -v 4 显示详细日志
    #   模型通过 map.world 中的 <include> 直接加载
    # ============================================================
    gz_sim = ExecuteProcess(
        cmd=['gz', 'sim', '-r', '-v', '4', world_path],
        output='screen'
    )

    # ============================================================
    # robot_state_publisher (发布 /tf 和 /tf_static)
    #   Harmonic 中关节状态由 gz-sim-joint-state-publisher 发布
    # ============================================================
    robot_state_pub = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        name='robot_state_publisher',
        output='screen',
        parameters=[{
            'robot_description': robot_description,
            'publish_frequency': 30.0,
        }]
    )

    # ============================================================
    # ros_gz_bridge 桥接节点 (Gazebo传感器 -> ROS2话题)
    # ============================================================
    # 优先使用 car_sim 包内的 bridge.yaml
    bridge_yaml_path = os.path.join(car_sim_src, 'bridge.yaml')
    if not os.path.isfile(bridge_yaml_path):
        bridge_yaml_path = os.path.normpath(
            os.path.join(car_sim_share, '..', '..', '..', '..', '..',
                         'config', 'ros_gz_bridge.yaml')
        )
    if not os.path.isfile(bridge_yaml_path):
        bridge_yaml_path = os.path.normpath(
            os.path.join(get_package_share_directory('car_sim'), '..', '..',
                         '..', '..', 'percep_node+track', 'config', 'ros_gz_bridge.yaml')
        )

    print(f'[INFO] bridge YAML 路径: {bridge_yaml_path} (exists={os.path.isfile(bridge_yaml_path)})')

    ros_gz_bridge = Node(
        package='ros_gz_bridge',
        executable='parameter_bridge',
        name='ros_gz_bridge',
        output='screen',
        arguments=[
            '--ros-args',
            '-p', f'config_file:={bridge_yaml_path}',
        ]
    )

    # ============================================================
    # sim_perception 感知节点 (可选)
    # ============================================================
    perception_node = Node(
        package='sim_perception',
        executable='sim_node',
        name='sim_node',
        output='screen',
        condition=IfCondition(with_perception)
    )

    # ============================================================
    # 构建 LaunchDescription
    # ============================================================
    return LaunchDescription([
        declare_with_perception,
        set_gz_resource_path,
        gz_sim,
        robot_state_pub,
        ros_gz_bridge,
        perception_node,
    ])
