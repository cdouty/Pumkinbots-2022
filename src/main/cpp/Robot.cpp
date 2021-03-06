// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include <frc/Joystick.h>
#include <frc/TimedRobot.h>
#include <frc/Timer.h>
#include <frc/motorcontrol/PWMSparkMax.h>
#include <frc/motorcontrol/MotorControllerGroup.h>
#include <frc/drive/DifferentialDrive.h>

#include "IntakeSubsystem.h"

/**
 * This sample program shows how to control a motor using a joystick. In the
 * operator control part of the program, the joystick is read and the value is
 * written to the motor.
 *
 * Joystick analog values range from -1 to 1 and speed controller inputs as
 * range from -1 to 1 making it easy to work together.
 */
class Robot : public frc::TimedRobot
{
public:
  Robot()
  {
    m_left.SetInverted(true);
    m_robotDrive.SetExpiration(100_ms);
    m_timer.Start();
  }

  void AutonomousInit() override
  {
    m_intake.Init();
    m_timer.Reset();
    m_timer.Start();
  }

  void AutonomousPeriodic() override
  {
    // Drive for 2 seconds
    if (m_timer.Get() < 2_s)
    {
      // Drive forwards half speed
      m_robotDrive.ArcadeDrive(0.5, 0.0);
    }
    else
    {
      // Stop robot
      m_robotDrive.ArcadeDrive(0.0, 0.0);
    }
  }

  void TeleopInit() override 
  {
    m_intake.Init();
  }

  void TeleopPeriodic() override
  {
    // Drive with arcade style (use right stick)
    // Stick forward is -1 on the Y-axis, so invert the signal.
    m_robotDrive.ArcadeDrive(-m_stick.GetY(), m_stick.GetX());
    // Check and run the IntakeSubsystem.
    m_intake.RunPeriodic();
  }

  void TestInit() override
  {
    // Disable to drive motors in Test mode so that the robot stays on the bench.
    m_robotDrive.StopMotor();
    m_intake.Init();
  }

  void TestPeriodic() override
  {
    // Only run the intake subsystem in Test mode.
    m_intake.RunPeriodic();
  }

private:
  frc::PWMSparkMax m_rightA{0}; // The number is the PWM channel on the rio.
  frc::PWMSparkMax m_rightB{2};
  frc::PWMSparkMax m_leftA{1};
  frc::PWMSparkMax m_leftB{3};
  frc::MotorControllerGroup m_right{m_rightA, m_rightB};
  frc::MotorControllerGroup m_left{m_leftA, m_leftB};
  frc::DifferentialDrive m_robotDrive{m_left, m_right};
  // Sample intake motor controller in PWM mode.
  frc::PWMSparkMax m_intakeDrive{4};

  frc::Joystick m_stick{0};
  frc::Timer m_timer;

  // Create an IntakeSubsystem to encapsulate the behavior.
  // This object must be created after the objects that it uses.
  // Bind the intake on/off to joystick button 2.
  IntakeSubsystem m_intake{2, m_intakeDrive, m_stick};
};

#ifndef RUNNING_FRC_TESTS
int main()
{
  return frc::StartRobot<Robot>();
}
#endif
