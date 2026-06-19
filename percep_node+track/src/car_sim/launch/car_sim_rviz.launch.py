#!/usr/bin/env python3
"""
car_sim_rviz.launch.py  —  RViz2 可视化启动文件
================================================
功能:
  1. 从 xacro 加载机器人模型 (robot_description)
  2. 启动 robot_state_publisher 发布 TF
  3. 启动 RViz2，预配置了以下显示:
     - 3D 网格 (Grid)
     - TF 坐标系树
     - 车辆 3D 模型 (RobotModel)
     - 相机图像 (/camera/image_raw)
     - 激光雷达扫描 (/scan)
  4. (可选) 启动 ros_gz_bridge 桥接传感器数据

运行方式:
  # 方式 A: 单独启动 RViz2（假设 Gazebo + bridge 已在运行）
  ros2 launch car_sim car_sim_rviz.launch.py

  # 方式 B: 同时启动 bridge（连接已有 Gazebo 仿真）
  ros2 launch car_sim car_sim_rviz.launch.py with_bridge:=true
"""

import os
import sys

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
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
        print(f'[WARN] 无法通过 ament 找到 car_sim, 回退到: {car_sim_share}')

    car_sim_src = os.path.normpath(
        os.path.join(car_sim_share, '..', '..', '..', 'src', 'car_sim')
    )
    if not os.path.isdir(car_sim_src):
        car_sim_src = car_sim_share

    car_sim_urdf = os.path.join(car_sim_src, 'urdf')

    # ============================================================
    # Launch 参数
    # ============================================================
    with_bridge = LaunchConfiguration('with_bridge', default='false')

    declare_with_bridge = DeclareLaunchArgument(
        'with_bridge', default_value='false',
        description='是否同时启动 ros_gz_bridge（需已有 Gazebo 仿真在运行）'
    )

    # ============================================================
    # Xacro -> URDF (robot_description)
    # ============================================================
    import xacro

    test_xacro_path = os.path.join(car_sim_urdf, 'test.xacro')
    orig_xacro_path = os.path.join(car_sim_urdf, 'car_sim.xacro')

    if os.path.isfile(test_xacro_path):
        xacro_file = test_xacro_path
    else:
        xacro_file = orig_xacro_path

    print(f'[INFO] 加载 xacro 文件: {xacro_file}')
    doc = xacro.process_file(xacro_file)
    robot_description = doc.toxml()
    print(f'[INFO] URDF 生成成功, 长度: {len(robot_description)} 字符')

    # ============================================================
    # RViz2 配置文件
    # ============================================================
    rviz_config_path = os.path.join(car_sim_src, 'config', 'car_sim.rviz')
    if not os.path.isfile(rviz_config_path):
        # 回退到包共享目录
        rviz_config_path = os.path.join(car_sim_share, 'config', 'car_sim.rviz')
    if not os.path.isfile(rviz_config_path):
        print(f'[WARN] 未找到 RViz 配置文件: {rviz_config_path}')
        print(f'[WARN] RViz2 将以默认配置启动')
        rviz_config_path = None

    if rviz_config_path:
        print(f'[INFO] RViz 配置: {rviz_config_path}')

    # ============================================================
    # robot_state_publisher (发布 /tf 和 /tf_static)
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
    # RViz2
    # ============================================================
    rviz_args = []
    if rviz_config_path:
        rviz_args = ['-d', rviz_config_path]

    rviz2_node = Node(
        package='rviz2',
        executable='rviz2',
        name='rviz2',
        output='screen',
        arguments=rviz_args
    )

    # ============================================================
    # ros_gz_bridge (可选) — 连接 Gazebo 传感器数据
    # ============================================================
    bridge_yaml_path = os.path.join(car_sim_src, 'bridge.yaml')
    if not os.path.isfile(bridge_yaml_path):
        bridge_yaml_path = os.path.normpath(
            os.path.join(car_sim_share, '..', '..', '..', '..', '..',
                         'config', 'ros_gz_bridge.yaml')
        )
    if not os.path.isfile(bridge_yaml_path):
        bridge_yaml_path = os.path.normpath(
            os.path.join(car_sim_share, '..', '..', '..', '..',
                         'percep_node+track', 'config', 'ros_gz_bridge.yaml')
        )

    print(f'[INFO] bridge YAML: {bridge_yaml_path} (exists={os.path.isfile(bridge_yaml_path)})')

    ros_gz_bridge = Node(
        package='ros_gz_bridge',
        executable='parameter_bridge',
        name='ros_gz_bridge',
        output='screen',
        arguments=[
            '--ros-args',
            '-p', f'config_file:={bridge_yaml_path}',
        ],
        condition=IfCondition(with_bridge)
    )

    # ============================================================
    # 构建 LaunchDescription
    # ============================================================
    return LaunchDescription([
        declare_with_bridge,
        robot_state_pub,
        rviz2_node,
        ros_gz_bridge,
    ])
