"""
步骤：
  1. 导入库；
  2. 定义节点类；
    2-1. 创建发布方（向 /turtle1/cmd_vel 发布 Twist）；
    2-2. 创建定时器（周期发布速度指令）；
    2-3. 在回调中切换两个圆的运动状态（正向圆 / 反向圆）。
  3. 编写 main 函数；
  4. 入口点调用 main。
"""

import math
from enum import Enum

import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist


class State(Enum):
    CIRCLE1 = 1   # 正向圆（逆时针）
    CIRCLE2 = 2   # 反向圆（顺时针）
    STOP = 3      # 停止


class TurtleFigure8(Node):
    def __init__(self):
        super().__init__('turtle_figure8')

        self._state = State.CIRCLE1  # 先正转
        self._elapsed = 0.0          # 已运行时间（累计）

        # 从参数读取线速度和角速度，支持 launch / yaml 配置
        self.declare_parameter('linear_speed', 2.0)
        self.declare_parameter('angular_speed', 2.0)

        # 2-1. 创建发布方：向 /turtle1/cmd_vel（乌龟速度控制）发布 Twist 消息
        self._publisher = self.create_publisher(
            Twist, '/turtle1/cmd_vel', 10)

        # 2-2. 创建定时器：每 0.05s 触发一次回调
        self._timer = self.create_timer(0.05, self.time_callback)

        angular_speed = self.get_parameter('angular_speed').value
        # 计算每个圆的运行时间 T = 2π / ω
        self._circle_time = 2.0 * math.pi / abs(angular_speed)

        self.get_logger().info(
            f'8字环节点启动: 线速度={self.get_parameter("linear_speed").value:.2f}, '
            f'角速度={angular_speed:.2f}, 每圈时间={self._circle_time:.2f}s')
        self.get_logger().info(
            f'第一阶段：正向圆（逆时针），持续 {self._circle_time:.2f} 秒')

    def time_callback(self):
        """定时器回调：根据当前状态发布速度指令"""
        msg = Twist()
        linear = self.get_parameter('linear_speed').value
        angular = abs(self.get_parameter('angular_speed').value)

        # [修复] 使用 self._elapsed 而非 self._total_time
        self._elapsed += 0.05

        if self._state == State.CIRCLE1:
            msg.linear.x = linear
            msg.angular.z = angular

            if self._elapsed >= self._circle_time:
                self.get_logger().info(
                    '第一阶段完成，切换为反向圆（顺时针）')
                self._state = State.CIRCLE2
                self._elapsed = 0.0  # 重置计时

        elif self._state == State.CIRCLE2:
            msg.linear.x = linear
            msg.angular.z = -angular  # 反转 z 轴 = 顺时针

            if self._elapsed >= self._circle_time:
                self.get_logger().info(
                    '第二阶段完成，8字形轨迹绘制完毕，乌龟停止！')
                self._state = State.STOP

        elif self._state == State.STOP:
            msg.linear.x = 0.0
            msg.angular.z = 0.0
            self._timer.cancel()
            self.get_logger().info('节点工作完成，定时器已停止。')

        self._publisher.publish(msg)  # 发布速度指令


def main(args=None):
    rclpy.init(args=args)
    node = TurtleFigure8()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
