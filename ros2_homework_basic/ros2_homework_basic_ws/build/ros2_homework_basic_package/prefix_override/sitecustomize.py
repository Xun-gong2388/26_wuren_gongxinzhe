import sys
if sys.prefix == '/usr':
    sys.real_prefix = sys.prefix
    sys.prefix = sys.exec_prefix = '/home/xun/桌面/ros2/ros2_homework/ros2_homework_basic/ros2_homework_basic_ws/install/ros2_homework_basic_package'
