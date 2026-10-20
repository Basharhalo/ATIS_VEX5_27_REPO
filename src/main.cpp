/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Bashar                                                     */
/*    Created:      10/17/2026, 10:10:56 PM                                    */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

// Function and device definitions

// Brain and Controller
brain Brain;
controller Controller1 = controller(primary);

// Drivetrain Motors
motor DT_leftMotor = motor(PORT1, ratio18_1, false);
motor DT_rightMotor = motor(PORT2, ratio18_1, true);

// Claw Grip Motors
motor ClawGrip1_upperMotor = motor(PORT3, ratio36_1, false);
motor ClawGrip2_bottomMotor = motor(PORT4, ratio36_1, true);

// Claw Arm Motor
motor ClawArm = motor(PORT5, ratio36_1, false);

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void)
{

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void)
{
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
}


void usercontrol(void)
{
  // User control code here, inside the loop
  while (1)
  {
    // Left joystick for turning and driving (Setting up values)
    int forwardSpeed = Controller1.Axis3.position();
    int turnSpeed = Controller1.Axis4.position();

    // Left joystick for turning and driving | Left Joystick
    DT_leftMotor.spin(forward, forwardSpeed + turnSpeed, percent);
    DT_rightMotor.spin(forward, forwardSpeed - turnSpeed, percent);

// ------------------------------------------------------------------------------------

    // Right joystick for controlling arm length | Right Joystick
    int armspeed = Controller1.Axis2.position();
    
    if (abs(armspeed) > 5)
    {
    ClawArm.spin(forward, armspeed, percent);
    }
      else
     {
       ClawArm.stop(hold);
     }
// ------------------------------------------------------------------------------------

    // Controls  Claw Grip 1 | R1 R2
    if (Controller1.ButtonR1.pressing())
    {
      ClawGrip1_upperMotor.spin(forward, 80, percent);
    }
      else if (Controller1.ButtonR2.pressing())
      {
        ClawGrip1_upperMotor.spin(reverse, 80, percent);
      }
         else
        {
         ClawGrip1_upperMotor.stop(hold);
        }

// ------------------------------------------------------------------------------------
    // Controls  Claw Grip 2 | L1 L2

    if (Controller1.ButtonL1.pressing())
      {
      ClawGrip2_bottomMotor.spin(forward, 80, percent);
      }
       else if (Controller1.ButtonL2.pressing())
      {
        ClawGrip2_bottomMotor.spin(reverse, 80, percent);
      }
         else
        {
         ClawGrip2_bottomMotor.stop(hold);
        }
// ------------------------------------------------------------------------------------
    wait(20, msec); // Sleep the task to prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main()
{
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true)
  {
    wait(100, msec);
  }
}
