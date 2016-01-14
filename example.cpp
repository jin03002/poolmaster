// /** Simple example of using the FastFiz library without writing an AI
//  */
// #include "FastFiz.h"
// #include "Rules.h"
// #include "LogFile.h"
// #include <iostream>
// #include <string>
// #include <map>
// #include <cstdlib>
// #include <ctime>
//
// using namespace Pool;
//
// int main(int argc, char * const argv[]) {
//   // Output version info
//   cerr << getFastFizVersion() << endl;
//   cerr << getRulesVersion() << endl;
//
//   // Generate Gaussian noise using tournament values.
//   GaussianNoise gn;
//
//   // Create a new eight ball state with no time limits.
//   EightBallState myState(0,0);
//   GameState& st = myState;
//   cerr << "Initial racked Game State is: " << st << endl;
//
//   // Start a log file
//   LogWriter lw("example.log",GT_EIGHTBALL,&gn,"Example Agent");
//
//   // A typical break shot
//   GameShot myShot = {ShotParams(0.0,0.0,25.0,270.0,5.0), // Shot parameters
//     0.48,1.67705, // Cue position
//     Ball::UNKNOWN_ID,Table::UNKNOWN_POCKET, // No need to call ball/pocket
//     DEC_NO_DECISION, // No decision to make.
//     0.0}; // No time spent.
//
//   // Noise is added, shot executed, and log written.
//   ShotResult res = lw.LogAndExecute(st,myShot);
//
//   cerr << "Shot result is: " << res << endl;
//
//   if (res == SR_OK) { // If you kept your turn
//     lw.write(st);
//     srand ( time(NULL) );
//     GameShot myNextShot = {
//       ShotParams(0.0,0.0,25.0,(360.0*rand())/RAND_MAX,5.0), // random direction
//       0.0, 0.0, // cue position not needed
//       Ball::THREE,Table::SW, // call a ball and a pocket
//       DEC_NO_DECISION, // No decision to make.
//       0.0}; // No time spent.
//     lw.write(myNextShot); // write shot to log file, even if not successful.
//     res = st.executeShot(myNextShot); // Execute the shot on the table (no noise added).
//     cerr << "Second shot result is " << res << endl;
//   }
//
//   lw.write(st);
//   return 0;
// }

/** Our code
 */
#include "FastFiz.h"
#include "Rules.h"
#include "LogFile.h"
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace Pool;

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

int main(int argc, char * const argv[]) {
  ifstream file("OutputBalls.csv");

  pair<int, int> balls[16];
  string line;
  if (file.is_open()) {
    int i = 0;
    while (getline(file, line)) {
      vector<string> v = split(line, ',');
      balls[i] = make_pair(atoi(v[0].c_str()), atoi(v[1].c_str()));
    }
  }

  // Output version info
  cerr << getFastFizVersion() << endl;
  cerr << getRulesVersion() << endl;

  // Start a log file
  // LogWriter lw("example.log",GT_EIGHTBALL,&gn,"Example Agent");
  // Ball::Type blist=[Ball::CUE, Ball::ONE, Ball::TWO, Ball:: THREE, Ball::FOUR, Ball::FIVE,
  //                   Ball::SIX, Ball::SEVEN, Ball::EIGHT, Ball::NINE, Ball::TEN, Ball::ELEVEN,
  //                   Ball::TWELVE, Ball::THIRTEEN, Ball::FOURTEEN, Ball::FIFTEEN];

  TableState* ts = new TableState();
  // for (int i = 0; i < sizeof(balls)/; i++) {
  //   ts.setBall(blist[i], Ball::STATIONARY, balls[i][0], balls[i][1]);
  // }

  ts->setBall(Ball::CUE,      Ball::STATIONARY, balls[0].first, balls[0].second);
  ts->setBall(Ball::ONE,      Ball::STATIONARY, balls[1].first, balls[1].second);
  ts->setBall(Ball::TWO,      Ball::STATIONARY, balls[2].first, balls[2].second);
  ts->setBall(Ball::THREE,    Ball::STATIONARY, balls[3].first, balls[3].second);
  ts->setBall(Ball::FOUR,     Ball::STATIONARY, balls[4].first, balls[4].second);
  ts->setBall(Ball::FIVE,     Ball::STATIONARY, balls[5].first, balls[5].second);
  ts->setBall(Ball::SIX,      Ball::STATIONARY, balls[6].first, balls[6].second);
  ts->setBall(Ball::SEVEN,    Ball::STATIONARY, balls[7].first, balls[7].second);
  ts->setBall(Ball::EIGHT,    Ball::STATIONARY, balls[8].first, balls[8].second);
  ts->setBall(Ball::NINE,     Ball::STATIONARY, balls[9].first, balls[9].second);
  ts->setBall(Ball::TEN,      Ball::STATIONARY, balls[10].first, balls[10].second);
  ts->setBall(Ball::ELEVEN,   Ball::STATIONARY, balls[11].first, balls[11].second);
  ts->setBall(Ball::TWELVE,   Ball::STATIONARY, balls[12].first, balls[12].second);
  ts->setBall(Ball::THIRTEEN, Ball::STATIONARY, balls[13].first, balls[13].second);
  ts->setBall(Ball::FOURTEEN, Ball::STATIONARY, balls[14].first, balls[14].second);
  ts->setBall(Ball::FIFTEEN,  Ball::STATIONARY, balls[15].first, balls[15].second);

  // A typical break shot
  // GameShot myShot = {ShotParams(0.0, 0.0, 25.0, 270.0, 5.0), // Shot parameters
  //   0.48, 1.67705, // Cue position
  //   Ball::UNKNOWN_ID,Table::UNKNOWN_POCKET, // No need to call ball/pocket
  //   DEC_NO_DECISION, // No decision to make.
  //   0.0}; // No time spent.
  //
  // cerr << "Shot result is: " << res << endl;
  //
  // if (res == SR_OK) { // If you kept your turn
  //   srand ( time(NULL) );
  //   GameShot myNextShot = {
  //     ShotParams(0.0,0.0,25.0,(360.0*rand())/RAND_MAX,5.0), // random direction
  //     0.0, 0.0, // cue position not needed
  //     Ball::THREE,Table::SW, // call a ball and a pocket
  //     DEC_NO_DECISION, // No decision to make.
  //     0.0}; // No time spent.
  //   res = st.executeShot(myNextShot); // Execute the shot on the table (no noise added).
  // }

  return 0;
}
