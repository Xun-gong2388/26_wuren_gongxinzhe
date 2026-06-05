import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    
    pkg_share = get_package_share_directory('ros2_homework_basic_package')# 获取包的路径


    yaml_path = os.path.join(pkg_share, 'config', 'turtle_figure8.yaml')  #  YAML 参数文件路径

    # 启动 turtlesim 模拟器
    turtlesim_node = Node(
        package='turtlesim',
        executable='turtlesim_node',
        name='turtlesim',
        output='screen',
    )

    # 启动 8 字环控制节点（参数从 YAML 文件加载）
    turtle_figure8_node = Node(
        package='ros2_homework_basic_package',
        executable='turtle_figure8',  # 对应 setup.py 中 console_scripts 的入口点
        name='turtle_figure8',
        output='screen',
        parameters=[yaml_path],
    )

    return LaunchDescription([
        turtlesim_node,
        turtle_figure8_node,
    ])
