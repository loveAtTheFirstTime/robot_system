/*
 Copyright 2024 ROS2 LLC

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#include "stanley_controller/stanley_controller.hpp"
#include "stanley_controller/logger.hpp"

namespace stanley_controller
{
StanleyController::StanleyController() : Node("stanley_controller")
{
     current_pose_ = this->create_subscription<geometry_msgs::msg::Vector3Stamped>(
          "estimate_pose",
          rclcpp::QoS(rclcpp::KeepLast(1)).transient_local().reliable(),
          std::bind(&StanleyController::currentPoseCallback, this, std::placeholders::_1));
     path_sub_ = this->create_subscription<algorithm_msgs::msg::Path>(
          "algorithm_path",
          rclcpp::QoS(rclcpp::KeepLast(1)).transient_local().reliable(),
          std::bind(&StanleyController::pathSubCallback, this, std::placeholders::_1));
     vel_ = this->create_publisher<geometry_msgs::msg::Twist>(
          "cmd_vel",
          10);
}

StanleyController::~StanleyController() {}

void StanleyController::sendVelocity(const double v, const double w)
{
     auto msg = geometry_msgs::msg::Twist();
     msg.linear.x = v;
     msg.angular.z = w;
     vel_->publish(msg);
}

void StanleyController::pathSubCallback(const algorithm_msgs::msg::Path::SharedPtr msg)
{
     path_ = *msg;
     if (is_path_received_) {
          return;
     }
     for (size_t i = 0; i < path_.segments.size(); i++) {
          if (path_.segments[i].type == path_.segments[i].LINE) {
               std::cout << "Path " << i << " : " 
                    << "start[" << path_.segments[i].line.p0.x << ", " << path_.segments[i].line.p0.y << "] "
                    << "end[" << path_.segments[i].line.p1.x << ", " << path_.segments[i].line.p1.y << "]" << std::endl;
          } else if (path_.segments[i].type == path_.segments[i].BEZIER3) {
               std::cout << "Path " << i << " : " 
                    << "p0[" << path_.segments[i].bezier3.p0.x << ", " << path_.segments[i].bezier3.p0.y << "] "
                    << "p1[" << path_.segments[i].bezier3.p1.x << ", " << path_.segments[i].bezier3.p1.y << "] "
                    << "p2[" << path_.segments[i].bezier3.p2.x << ", " << path_.segments[i].bezier3.p2.y << "] "
                    << "p3[" << path_.segments[i].bezier3.p3.x << ", " << path_.segments[i].bezier3.p3.y << "]" << std::endl;
          } else if (path_.segments[i].type == path_.segments[i].BEZIER5) {
               std::cout << "Path " << i << " : " 
                    << "p0[" << path_.segments[i].bezier5.p0.x << ", " << path_.segments[i].bezier5.p0.y << "] "
                    << "p1[" << path_.segments[i].bezier5.p1.x << ", " << path_.segments[i].bezier5.p1.y << "] "
                    << "p2[" << path_.segments[i].bezier5.p2.x << ", " << path_.segments[i].bezier5.p2.y << "] "
                    << "p3[" << path_.segments[i].bezier5.p3.x << ", " << path_.segments[i].bezier5.p3.y << "] "
                    << "p4[" << path_.segments[i].bezier5.p4.x << ", " << path_.segments[i].bezier5.p4.y << "] "
                    << "p5[" << path_.segments[i].bezier5.p5.x << ", " << path_.segments[i].bezier5.p5.y << "]" << std::endl;
          }
     }
     is_path_received_ = true;
}

void StanleyController::currentPoseCallback(const geometry_msgs::msg::Vector3Stamped::SharedPtr msg)
{
     if (!is_path_received_) {
          std::cout << getCurrentTime() << "path is empty, return" << std::endl;
          sendVelocity(0.0f, 0.0f);
          return;
     }
}




}  // namespace stanley_controller
