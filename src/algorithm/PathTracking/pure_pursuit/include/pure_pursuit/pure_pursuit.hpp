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

#ifndef PURE_PURSUIT__PURE_PURSUIT_HPP_
#define PURE_PURSUIT__PURE_PURSUIT_HPP_

#include <cmath>
#include <vector>
#include <chrono>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "pure_pursuit/logger.hpp"
#include "geometry_msgs/msg/vector3_stamped.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "algorithm_msgs/msg/path.hpp"
#include "algorithm_msgs/msg/line.hpp"
#include "algorithm_msgs/msg/bezier3.hpp"
#include "algorithm_msgs/msg/bezier5.hpp"

namespace pure_pursuit
{
struct Vector {
    double vx;
    double vy;
};
struct Pose {
    double x;
    double y;
    double yaw;
};
class PurePursuit : public rclcpp::Node
{
public:
    PurePursuit();
    ~PurePursuit();

private:
    void initParam();
    void initFirstValue();
    double normalizeAngle(double angle);
    void currentPoseCallback(const geometry_msgs::msg::Vector3Stamped::SharedPtr msg);
    void sendVelocity(const double v, const double w);
    void pathSubCallback(const algorithm_msgs::msg::Path::SharedPtr msg);
    void calculateTargetOnLine(Pose & target, const double lookahead, const Pose & current, const algorithm_msgs::msg::Line line);
    void calculateTargetOnBezier3(Pose & target, const double lookahead, const Pose & current, const algorithm_msgs::msg::Bezier3 bezier3);
    void calculateTargetOnBezier5(Pose & target, const double lookahead, const Pose & current, const algorithm_msgs::msg::Bezier5 bezier5);
    
    rclcpp::Subscription<geometry_msgs::msg::Vector3Stamped>::SharedPtr current_pose_;
    double lookaheaddist_{0.0f};
    double k_{0.0f};
    double max_v_{1.0f};
    double max_w_{M_PI_2};
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr vel_;
    rclcpp::Subscription<algorithm_msgs::msg::Path>::SharedPtr path_sub_;

    double v_;
    double w_;

    bool is_path_received_{false};
    algorithm_msgs::msg::Path path_;

};
}  // namespace pure_pursuit
#endif  // PURE_PURSUIT__PURE_PURSUIT_HPP_
