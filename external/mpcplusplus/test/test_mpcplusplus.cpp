
#include "mpcplusplus/mpcplusplus.h"
#include <Eigen/Dense>
#include <iostream>
#include <math.h>
#include <limits>

#include <gtest/gtest.h>

const double INF = std::numeric_limits<double>::max();

TEST(TestUseCase, droneVariable) {

  // Configurable parameters
  const int Nu = 5; // Appended gravity term
  const int Nx = 6; // Number of states
  const int N = 10;   // Time horizons to consider
  const double dt = 0.1;             // Time horizon
  const int m = 1;                   // Mass of drone

  // Weights on state deviation and control input
  Eigen::MatrixXd Qx(Nx, Nx);
  Qx.setZero();
  Qx.diagonal() << 100, 100, 1000, 1, 1, 1;
  Eigen::MatrixXd Qn = 5 * Qx;
  Eigen::MatrixXd Ru(Nu, Nu);
  Ru.setZero();
  Ru.diagonal() << 1, 1, 1, 1, 1;

  // Linearized dynamics from drone
  Eigen::MatrixXd Ad(Nx, Nx);
  Ad.row(0) << 1, 0, 0, dt, 0, 0;
  Ad.row(1) << 0, 1, 0, 0, dt, 0;
  Ad.row(2) << 0, 0, 1, 0, 0, dt;
  Ad.row(3) << 0, 0, 0, 1, 0, 0;
  Ad.row(4) << 0, 0, 0, 0, 1, 0;
  Ad.row(5) << 0, 0, 0, 0, 0, 1;

  Eigen::MatrixXd Bd(Nx, Nu);
  Bd.row(0) << 0, 0, 0, 0, 0;
  Bd.row(1) << 0, 0, 0, 0, 0;
  Bd.row(2) << 0, 0, 0, 0, 0;
  Bd.row(3) << -9.8 * dt, 0, 0, 0, 0;
  Bd.row(4) << 0, 9.8 * dt, 0, 0, 0;
  Bd.row(5) << 0, 0, 0, dt / m, -9.8 * dt;

  std::vector<Eigen::MatrixXd> Ad_vec;
  std::vector<Eigen::MatrixXd> Bd_vec;
  std::vector<Eigen::MatrixXd> Q_vec;
  std::vector<Eigen::MatrixXd> U_vec;

  Ad_vec.resize(N);
  Bd_vec.resize(N);
  Q_vec.resize(N+1);
  U_vec.resize(N);

  Eigen::MatrixXd ref_traj(Nx,N+1);
  Eigen::VectorXd initial_state(Nx);
  initial_state << 0,0,0,0,0,0;

  for (int i = 0; i < N; ++i) {
    Ad_vec.at(i) = Ad;
    Bd_vec.at(i) = Bd;
    Q_vec.at(i) = Qx;
    U_vec.at(i) = Ru;
    ref_traj.col(i) = initial_state;
  }
  Q_vec.back() = Qx;
  ref_traj.col(N) = initial_state;

  mpcplusplus::LinearMPC mpc(N,Nx,Nu);

  mpc.update_weights(Q_vec,U_vec);
  mpc.update_dynamics(Ad_vec,Bd_vec);

  Eigen::MatrixXd x_out;
  double f_val;
  mpc.solve(initial_state, ref_traj, x_out, f_val);
}

// Run all the tests that were declared with TEST()
int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}