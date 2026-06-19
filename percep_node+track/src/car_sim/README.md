# car_sim — 仿真小车使用指南

## 前置条件

```bash
# 1. source ROS 2 环境
source /opt/ros/humble/setup.bash    # 按你的 ROS 2 版本替换

# 2. source 工作空间
cd ~/ros2_ws/ros2_ws
source install/setup.bash

---

## 一、启动 Gazebo 仿真（车辆 + 赛道）

```bash
cd launch
./launch_sim.sh
```

> 这会自动完成：xacro→URDF → 启动 Gazebo → 启动 ros_gz_bridge

---

## 二、启动 RViz2 可视化（新终端）

```bash
# 需重新 source
source /opt/ros/humble/setup.bash
source ~/ros2_ws/ros2_ws/install/setup.bash

cd launch
./car_sim_rviz.launch.py    
```

> RViz2 已预配置：RobotModel / TF / 相机图像 / 激光雷达扫描

---

## 三、键盘控制小车（新终端）

```bash
ros2 run teleop_twist_keyboard teleop_twist_keyboard --ros-args -r cmd_vel:=/cmd_vel
```

| 按键 | 动作 |
|------|------|
| `i` | 前进 |
| `,` | 后退 |
| `j` | 左转 |
| `l` | 右转 |
| `k` | 停止 |
| `q` | 退出 |

---

## 四、查看传感器数据

```bash
# 列出所有 ROS 2 话题
ros2 topic list

# 相机图像
ros2 topic echo /camera/image_raw --no-arr

# 激光雷达
ros2 topic echo /scan

# IMU
ros2 topic echo /imu/data

# GPS
ros2 topic echo /gps/fix

# 里程计
ros2 topic echo /odom

# 速度指令（查看发给车的命令）
ros2 topic echo /cmd_vel

# 话题发布频率
ros2 topic hz /camera/image_raw
ros2 topic hz /scan
```

### 查看 Gazebo 内部话题

```bash
# 列出所有 Gazebo 话题
gz topic -l

# 直接监听相机
gz topic -e -t /world/map/model/shixi_car/link/camera_link/sensor/camera/image
```

---

## 话题总览

| 传感器 | ROS 2 话题 | 类型 |
|--------|-----------|------|
| 相机 | `/camera/image_raw` | `sensor_msgs/Image` |
| 激光雷达 | `/scan` | `sensor_msgs/LaserScan` |
| IMU | `/imu/data` | `sensor_msgs/Imu` |
| GPS | `/gps/fix` | `sensor_msgs/NavSatFix` |
| 磁力计 | `/magnetic_field` | `sensor_msgs/MagneticField` |
| 里程计 | `/odom` | `nav_msgs/Odometry` |
| 控制指令 | `/cmd_vel` (发布) | `geometry_msgs/Twist` |