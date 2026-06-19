import sys
if sys.prefix == '/usr':
    sys.real_prefix = sys.prefix
    sys.prefix = sys.exec_prefix = '/home/xun/桌面/ros2_ws/ros2_ws/ros2_ws/percep_node+track/install/car_sim'
