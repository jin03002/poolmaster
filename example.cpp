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
#include <vector>
#include <string>
#include <map>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace Pool;

// For splitting strings
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

// For printing vectors
template<typename T>
ostream& operator<< (ostream& out, const vector<T>& v) {
    out << "[";
    size_t last = v.size() - 1;
    for(size_t i = 0; i < v.size(); ++i) {
        out << v[i];
        if (i != last)
            out << ", ";
    }
    out << "]";
    return out;
}

void simulateShots(TableState* ts) {
    string initialState = ts->toString();
    vector<string> tableStates;

    ShotParams params = ShotParams(0.1, 0.1, 25.3, 274.0, 2.0);
    Shot* shot = (*ts).executeShot(params);

    for (int i = 0; i < 360; i++) {
        TableState* start = new TableState();
        start->fromString(initialState);
        ShotParams params = ShotParams(0.0, 0.0, 20, i, 3.0);
        try {
            Shot* shot = start->executeShot(params);
            tableStates.push_back(start->toString());
        } catch (BadShotException e) {
            cout << "shot " << i << " skipped: " << e.getTypeString() << endl;
        } catch (char const* msg) {
            cout << "shot " << i << " skipped: " << msg << endl;
        }
    }
    cout << tableStates.size() << " shots taken" << endl;
}

void writeTableFile(TableState* ts) {
    ofstream outFile;
    outFile.open("out_state.csv");
    outFile << ts->getBall(Ball::CUE).getPos().x << "," << ts->getBall(Ball::CUE).getPos().y << "," << 0.028575 << endl;
    outFile << ts->getBall(Ball::ONE).getPos().x << "," << ts->getBall(Ball::ONE).getPos().y << "," << 0.028575 << endl;
    outFile << ts->getBall(Ball::TWO).getPos().x << "," << ts->getBall(Ball::TWO).getPos().y << "," << 0.028575 << endl;
    outFile << ts->getBall(Ball::THREE).getPos().x << "," << ts->getBall(Ball::THREE).getPos().y << "," << 0.028575 << endl;
    outFile << ts->getBall(Ball::FOUR).getPos().x << "," << ts->getBall(Ball::FOUR).getPos().y << "," << 0.028575 << endl;
    outFile << ts->getBall(Ball::FIVE).getPos().x << "," << ts->getBall(Ball::FIVE).getPos().y << "," << 0.028575 << endl;
    outFile << ts->getBall(Ball::SIX).getPos().x << "," << ts->getBall(Ball::SIX).getPos().y << "," << 0.028575 << endl;
    outFile << ts->getBall(Ball::SEVEN).getPos().x << "," << ts->getBall(Ball::SEVEN).getPos().y << "," << 0.028575 << endl;
    outFile << ts->getBall(Ball::EIGHT).getPos().x << "," << ts->getBall(Ball::EIGHT).getPos().y << "," << 0.028575 << endl;
    outFile << ts->getBall(Ball::NINE).getPos().x << "," << ts->getBall(Ball::NINE).getPos().y << "," << 0.028575 << endl;
    outFile << ts->getBall(Ball::TEN).getPos().x << "," << ts->getBall(Ball::TEN).getPos().y << "," << 0.028575 << endl;
    outFile << ts->getBall(Ball::ELEVEN).getPos().x << "," << ts->getBall(Ball::ELEVEN).getPos().y << "," << 0.028575 << endl;
    outFile << ts->getBall(Ball::TWELVE).getPos().x << "," << ts->getBall(Ball::TWELVE).getPos().y << "," << 0.028575 << endl;
    outFile << ts->getBall(Ball::THIRTEEN).getPos().x << "," << ts->getBall(Ball::THIRTEEN).getPos().y << "," << 0.028575 << endl;
    outFile << ts->getBall(Ball::FOURTEEN).getPos().x << "," << ts->getBall(Ball::FOURTEEN).getPos().y << "," << 0.028575 << endl;
    outFile << ts->getBall(Ball::FIFTEEN).getPos().x << "," << ts->getBall(Ball::FIFTEEN).getPos().y << "," << 0.028575 << endl;

    outFile.close();
}

int main(int argc, char * const argv[]) {
  ifstream file("OutputBalls.csv");

  pair<int, int> balls[16];
  string line;
  if (file.is_open()) {
    int i = 0;
    while (getline(file, line) && i < sizeof(balls)/sizeof(*balls)) {
      vector<string> v = split(line, ',');
      balls[i] = make_pair(atoi(v[0].c_str()), atoi(v[1].c_str()));
      i++;
    }
  }

  TableState* ts = new TableState();

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

  simulateShots(ts);

  // ShotParams params = ;

  // try {
  //     Shot* shot = ts->executeShot(ShotParams(0.1, 0.1, 25.3, 274.0, 2.0));
  // } catch (BadShotException e) {
  //     cout << e.getTypeString() << endl;
  // } catch (char const* msg) {
  //     cout << msg << endl;
  // }

  writeTableFile(ts);

  return 0;
}
