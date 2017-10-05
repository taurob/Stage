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

//  if (verbose)
//    robot->pos->AddCallback(Model::CB_UPDATE, model_callback_t(PositionUpdate), robot);

//  robot->pos->Subscribe(); // starts the position updates

  // find a range finder
  
//  ModelRanger *laser = NULL;
//
//  printf( "\nWander ctrl for robot %s:\n",  robot->pos->Token() );
//  for( int i=0; i<16; i++ )
//    {
//      char name[32];
//      snprintf( name, 32, "ranger:%d", i ); // generate sequence of model names
//
//      printf( "  looking for a suitable ranger at \"%s:%s\" ... ", robot->pos->Token(), name );      
//      laser = dynamic_cast<ModelRanger *>(robot->pos->GetChild( name ));
//      
//      if( laser && laser->GetSensors()[0].sample_count > 8 )
//	{
//	  puts( "yes." );
//	  break;
//	}
//
//      puts( "no." );
//    }
//  
//  if( !laser ) {
//    PRINT_ERR("  Failed to find a ranger with more than 8 samples. Exit.");
//    exit(2);
//  }
//  
//  robot->laser = laser;
//  robot->laser->AddCallback(Model::CB_UPDATE, model_callback_t(LaserUpdate), robot);
//  robot->laser->Subscribe(); // starts the ranger updates

  wifi_ranger->AddCallback(Model::CB_UPDATE, model_callback_t(LaserUpdate), data);
  wifi_ranger->Subscribe();

  return 0; // ok
}

static std::string parent_name(const ModelWifiRanger *mod)
{
  Model* parent = mod->Parent();
  if (!parent)
  {
    return "";
  }

  return parent->name();
}

// inspect the ranger data and decide what to do
int LaserUpdate(Model *mod, struct callback_data* data)
{
  ModelWifiRanger* mwr = dynamic_cast<ModelWifiRanger*>(mod);
  if (mwr->wifis_in_range().size())
  {
    printf("'%s' connected to:\n", parent_name(mwr).c_str());

    std::set<ModelWifiRanger*>::iterator it;
    for (it = mwr->wifis_in_range().begin(); it != mwr->wifis_in_range().end(); ++it)
    {
      printf("'%s'\n", parent_name(*it).c_str());
    }
  }
//  // get the data
//  const std::vector<meters_t> &scan = robot->laser->GetSensors()[0].ranges;
//  uint32_t sample_count = scan.size();
//  if (sample_count < 1)
//    return 0;
//
//  bool obstruction = false;
//  bool stop = false;
//
//  // find the closest distance to the left and right and check if
//  // there's anything in front
//  double minleft = 1e6;
//  double minright = 1e6;
//
//  for (uint32_t i = 0; i < sample_count; i++) {
//    if (verbose)
//      printf("%.3f ", scan[i]);
//
//    if ((i > (sample_count / 3)) && (i < (sample_count - (sample_count / 3)))
//        && scan[i] < minfrontdistance) {
//      if (verbose)
//        puts("  obstruction!");
//      obstruction = true;
//    }
//
//    if (scan[i] < stopdist) {
//      if (verbose)
//        puts("  stopping!");
//      stop = true;
//    }
//
//    if (i > sample_count / 2)
//      minleft = std::min(minleft, scan[i]);
//    else
//      minright = std::min(minright, scan[i]);
//  }
//
//  if (verbose) {
//    puts("");
//    printf("minleft %.3f \n", minleft);
//    printf("minright %.3f\n ", minright);
//  }
//
//  if (obstruction || stop || (robot->avoidcount > 0)) {
//    if (verbose)
//      printf("Avoid %d\n", robot->avoidcount);
//
//    robot->pos->SetXSpeed(stop ? 0.0 : avoidspeed);
//
//    /* once we start avoiding, select a turn direction and stick
// with it for a few iterations */
//    if (robot->avoidcount < 1) {
//      if (verbose)
//        puts("Avoid START");
//      robot->avoidcount = random() % avoidduration + avoidduration;
//
//      if (minleft < minright) {
//        robot->pos->SetTurnSpeed(-avoidturn);
//        if (verbose)
//          printf("turning right %.2f\n", -avoidturn);
//      } else {
//        robot->pos->SetTurnSpeed(+avoidturn);
//        if (verbose)
//          printf("turning left %2f\n", +avoidturn);
//      }
//    }
//
//    robot->avoidcount--;
//  } else {
//    if (verbose)
//      puts("Cruise");
//
//    robot->avoidcount = 0;
//    robot->pos->SetXSpeed(cruisespeed);
//    robot->pos->SetTurnSpeed(0);
//  }
//
//  //  if( robot->pos->Stalled() )
//  // 	 {
//  // 		robot->pos->SetSpeed( 0,0,0 );
//  // 		robot->pos->SetTurnSpeed( 0 );
//  // }

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
