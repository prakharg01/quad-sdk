#ifndef INVERSE_DYNAMICS_H
#define INVERSE_DYNAMICS_H

#include <ros/ros.h>
#include <eigen3/Eigen/Eigen>
#include <spirit_utils/foot_jacobians.h>
#include <spirit_msgs/ControlInput.h>
#include <spirit_msgs/StateEstimate.h>
#include <spirit_msgs/MotorCommand.h>
#include <spirit_msgs/SwingLegPlan.h>
#include <spirit_msgs/FootstepPlan.h>


//! Implements inverse dynamics
/*!
   inverseDynamics implements inverse dynamics logic. It should expose a constructor that does any initialization required and an update method called at some frequency.
*/
class inverseDynamics {
  public:
	/**
	 * @brief Constructor for inverseDynamics
	 * @param[in] nh ROS NodeHandle to publish and subscribe from
	 * @return Constructed object of type inverseDynamics
	 */
	inverseDynamics(ros::NodeHandle nh);

	/**
	 * @brief Calls ros spinOnce and pubs data at set frequency
	 */
	void spin();
  
private:
		/**
   * @brief Callback function to handle new control input (GRF)
   * @param[in] Control input message contining ground reaction forces and maybe nominal leg positions
   */
	void controlInputCallback();
	void stateEstimateCallback();
	void swingLegPlanCallback();
	void footSteplsPlanCallback();
	void publishInverseDynamics();

	/// ROS subscriber for control input
	ros::Subscriber control_input_sub_;

	/// ROS subscriber for state estimate
	ros::Subscriber state_estimate_sub_;

	/// ROS subscriber for Swing Leg Plan
	ros::Subscriber swing_leg_plan_sub_;

	/// ROS publisher for inverse dynamics
	ros::Publisher foot_step_plan_pub_;

	/// Nodehandle to pub to and sub from
	ros::NodeHandle nh_;

	/// Update rate for sending and receiving data;
	double update_rate_;
};


#endif // MPC_CONTROLLER_H
