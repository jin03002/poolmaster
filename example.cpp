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
#include <tr1/tuple>
#include <string>
#include <map>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace Pool;

string bestState;
float bestAngle;

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

void writeAllTablesFile(vector<tr1::tuple<string, float, int> > tableStates) {
    ofstream outFile;
    outFile.open("tables.txt");

    for (std::vector<tr1::tuple<string, float, int> >::iterator it = tableStates.begin(); it != tableStates.end(); ++it) {
        // TableState* ts = tr1::get<0>(*it);
        outFile << "Table state: " << tr1::get<0>(*it) << " Angle: " << tr1::get<1>(*it) << " Balls pocketed: " << tr1::get<2>(*it) << endl;
    }
}

void simulateShots(TableState* ts) {
    string initialState = ts->toString();
    vector<tr1::tuple<string, float, int> > tableStates;
    int max = -1;
    bestState = "";

    for (int i = 0; i < 360; i+=1) {
        TableState* start = new TableState();
        start->fromString(initialState);
        ShotParams params = ShotParams(0.0, 0.0, 20, i, 4.5);
        try {
            Shot* shot = start->executeShot(params);
            int ballsPocketed = 0;
            for (std::vector<Ball>::iterator it = start->getBegin(); it != start->getEnd(); ++it) {
                if((*it).isPocketed())
                    ballsPocketed += 1;
                else if(!(*it).isInPlay())
                    exit(-1);
            }
            if (ballsPocketed > max){
                bestState = start->toString();
                max = ballsPocketed;
                bestAngle = i;
                cout << max << endl;
                // cout << bestState << endl;
            }
            tableStates.push_back(tr1::make_tuple(start->toString(), i, ballsPocketed));
        } catch (BadShotException e) {
            cout << "shot " << i << " skipped: " << e.getTypeString() << endl;
        } catch (char const* msg) {
            cout << "shot " << i << " skipped: " << msg << endl;
        }
    }
    cout << tableStates.size() << " shots taken" << endl;

    writeAllTablesFile(tableStates);
}

void writeTableFile(TableState* ts) {
    TableState* table = new TableState();
    table->fromString(bestState);
    ofstream outFile;
    outFile.open("out_state.csv");
    outFile << bestAngle << endl;
    outFile << table->getBall(Ball::CUE).getPos().x << "," << table->getBall(Ball::CUE).getPos().y << "," << 0.028575 << endl;
    outFile << table->getBall(Ball::ONE).getPos().x << "," << table->getBall(Ball::ONE).getPos().y << "," << 0.028575 << endl;
    outFile << table->getBall(Ball::TWO).getPos().x << "," << table->getBall(Ball::TWO).getPos().y << "," << 0.028575 << endl;
    outFile << table->getBall(Ball::THREE).getPos().x << "," << table->getBall(Ball::THREE).getPos().y << "," << 0.028575 << endl;
    outFile << table->getBall(Ball::FOUR).getPos().x << "," << table->getBall(Ball::FOUR).getPos().y << "," << 0.028575 << endl;
    outFile << table->getBall(Ball::FIVE).getPos().x << "," << table->getBall(Ball::FIVE).getPos().y << "," << 0.028575 << endl;
    outFile << table->getBall(Ball::SIX).getPos().x << "," << table->getBall(Ball::SIX).getPos().y << "," << 0.028575 << endl;
    outFile << table->getBall(Ball::SEVEN).getPos().x << "," << table->getBall(Ball::SEVEN).getPos().y << "," << 0.028575 << endl;
    outFile << table->getBall(Ball::EIGHT).getPos().x << "," << table->getBall(Ball::EIGHT).getPos().y << "," << 0.028575 << endl;
    outFile << table->getBall(Ball::NINE).getPos().x << "," << table->getBall(Ball::NINE).getPos().y << "," << 0.028575 << endl;
    outFile << table->getBall(Ball::TEN).getPos().x << "," << table->getBall(Ball::TEN).getPos().y << "," << 0.028575 << endl;
    outFile << table->getBall(Ball::ELEVEN).getPos().x << "," << table->getBall(Ball::ELEVEN).getPos().y << "," << 0.028575 << endl;
    outFile << table->getBall(Ball::TWELVE).getPos().x << "," << table->getBall(Ball::TWELVE).getPos().y << "," << 0.028575 << endl;
    outFile << table->getBall(Ball::THIRTEEN).getPos().x << "," << table->getBall(Ball::THIRTEEN).getPos().y << "," << 0.028575 << endl;
    outFile << table->getBall(Ball::FOURTEEN).getPos().x << "," << table->getBall(Ball::FOURTEEN).getPos().y << "," << 0.028575 << endl;
    outFile << table->getBall(Ball::FIFTEEN).getPos().x << "," << table->getBall(Ball::FIFTEEN).getPos().y << "," << 0.028575 << endl;

    outFile.close();
}

int main(int argc, char * const argv[]) {
  ifstream file("OutputShiftedCoords.csv");

  pair<float, float> balls[16];
  string line;
  if (file.is_open()) {
    int i = 0;
    while (getline(file, line) && i < sizeof(balls)/sizeof(*balls)) {
      vector<string> v = split(line, ',');
      balls[i] = make_pair(atof(v[0].c_str()), atof(v[1].c_str()));
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
