// This controller does nothing, just attaches to a wifi ranger. (Apparently,
// there needs to be a controller attached to a ranger for it to be updated by Stage.)

#include "stage.hh"
using namespace Stg;

static const double cruisespeed = 0.4;
static const double avoidspeed = 0.05;
static const double avoidturn = 0.5;
static const double minfrontdistance = 1.0; // 0.6
static const bool verbose = false;
static const double stopdist = 0.3;
static const int avoidduration = 10;

struct callback_data {
  ModelWifiRanger* wifi_ranger;
};

int LaserUpdate(Model *mod, struct callback_data* data);
//int PositionUpdate(Model *mod, struct callback_data* data);

// Stage calls this when the model starts up
extern "C" int Init(Model *mod, CtrlArgs *args)
{
  // local arguments
  printf("\nWifi controller initialised with:\n"
      "\tworldfile string \"%s\"\n"
      "\tcmdline string \"%s\"",
      args->worldfile.c_str(),
      args->cmdline.c_str() );

  ModelWifiRanger* wifi_ranger = dynamic_cast<ModelWifiRanger*>(mod);
  if (!wifi_ranger)
  {
    PRINT_ERR("No WifiRanger model given in wifi controller.");
    exit(1);
  }

  struct callback_data* data = new callback_data();
  data->wifi_ranger = wifi_ranger;

  wifi_ranger->AddCallback(Model::CB_UPDATE, model_callback_t(LaserUpdate), data);
  wifi_ranger->Subscribe();

  return 0; // ok
}

// inspect the ranger data and decide what to do
int LaserUpdate(Model *mod, struct callback_data* data)
{
  return 0; // run again
}

//int PositionUpdate(Model *, robot_t *robot)
//{
//  Pose pose = robot->pos->GetPose();
//
//  printf("Pose: [%.2f %.2f %.2f %.2f]\n", pose.x, pose.y, pose.z, pose.a);
//
//  return 0; // run again
//}
