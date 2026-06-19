#!/bin/bash
# 切换到脚本所在目录（确保相对路径正确）
cd "$(dirname "$0")"

echo "================================"
echo " car_sim Gazebo Harmonic 仿真启动"
echo "================================"

echo "[1/4] 生成 URDF ..."
xacro ../urdf/car_sim.xacro -o ../urdf/car_sim.urdf
if [ $? -eq 0 ]; then
  echo " URDF 生成成功"
else
  echo " [WARN] xacro 失败，继续尝试..."
fi

echo "[2/4] 设置模型搜索路径 ..."
# 注意：tracks 目录与 src 同级，所以从 car_sim/launch 出发需上两级
export GZ_SIM_RESOURCE_PATH="$(pwd)/../../tracks/models:$GZ_SIM_RESOURCE_PATH"
export GZ_SIM_RESOURCE_PATH="$(pwd)/models:$GZ_SIM_RESOURCE_PATH"
echo " GZ_SIM_RESOURCE_PATH=$GZ_SIM_RESOURCE_PATH"

echo "[3/4] 启动 Gazebo 并后台运行 ..."
# world 文件路径也修正为 ../../tracks/map.world
gz sim -r ../../tracks/map.world &
GZ_PID=$!
# 在启动 Gazebo 之前或之后添加
ros2 run robot_state_publisher robot_state_publisher --ros-args -p robot_description:="$(xacro ../urdf/car_sim.xacro)" &
echo " Gazebo PID = $GZ_PID"

echo "[4/4] 等待 Gazebo 就绪 (5秒)..."
sleep 5

echo "[5/4] 启动 ros_gz_bridge ..."
# 修正参数语法：使用 := 赋值，并用引号包裹路径
ros2 run ros_gz_bridge parameter_bridge --ros-args -p config_file:="$(pwd)/../bridge.yaml"

# 等待 Gazebo 结束（可选）
wait $GZ_PID