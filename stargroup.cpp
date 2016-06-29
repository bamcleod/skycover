#include "stargroup.h"
#include "probe.h"
#include <iostream>
#include <sstream>
using namespace std;

StarGroup::StarGroup() { }

StarGroup::~StarGroup() { }

StarGroup::StarGroup(vector<Star> _stars) {
    stars = _stars;
    update_mags();
}

void StarGroup::add_star(Star *_star) {
  stars.push_back(*_star);
}

Star StarGroup::star_at(int idx) {
  return stars[idx];
}

void StarGroup::update_mags() {
  for(vector<Star>::iterator it = stars.begin(); it != stars.end(); ++it) {
    mags.push_back(it->r);
  }
}

int int_frequency(int x, vector<int> l) {
  int freq = 0;
  for (vector<int>::iterator it = l.begin(); it != l.end(); ++it) {
    if (*it == x) {
      freq += 1;
    }
  }

  return freq;
}

int StarGroup::wfsmag() {
  vector<int>::iterator mag;
  for (mag=mags.begin(); mag != mags.end(); mag++) {
    if (int_frequency(*mag, mags) >= 3) {
      return *mag;
    }
  }

  return 0;
}

int StarGroup::gdrmag() {
  if (int_frequency(mags[0], mags) == 4) { return mags[0]; }

  vector<int>::iterator mag;
  for (mag=mags.begin(); mag != mags.end(); mag++) {
    if (int_frequency(*mag, mags) == 1) {
      return *mag;
    }
  }

  return 0;
}

string StarGroup::magpair() {
    std::ostringstream res;
    res << wfsmag() << ":" << gdrmag();

    return res.str();
}

int StarGroup::valid(int W, int G) {
  vector<double> angles;
  Point origin(0, 1);
  double angle;

  for (Star s : stars) {
    angles.push_back(angle_between_vectors(origin, s.point()));
    // cout << angles[angles.size()-1] << " ";
  }
  // cout << endl;

  if (quadrant(stars[0].point()) == 1 && quadrant(stars[3].point()) == 1) {
    if (angles[0] < angles[3]) { return 0; }
  }
  if (quadrant(stars[0].point()) == 2 && quadrant(stars[3].point()) == 2) {
    if (angles[0] > angles[3]) { return 0; }
  }
  if (quadrant(stars[0].point()) == 2 && quadrant(stars[3].point()) == 1) {
    return 0;
  }

  if (quadrant(stars[0].point()) == 1 && quadrant(stars[1].point()) == 1) {
    if (angles[0] > angles[1]) { return 0; }
  }
  if (quadrant(stars[0].point()) == 4 && quadrant(stars[1].point()) == 4) {
    if (angles[0] > angles[1]) { return 0; }
  }
  if (quadrant(stars[0].point()) == 4 && quadrant(stars[1].point()) == 1) {
    return 0;
  }

  if (quadrant(stars[1].point()) == 4 && quadrant(stars[2].point()) == 4) {
    if (angles[1] > angles[2]) { return 0; }
  }
  if (quadrant(stars[1].point()) == 3 && quadrant(stars[2].point()) == 3) {
    if (angles[2] > angles[1]) { return 0; }
  }
  if (quadrant(stars[1].point()) == 3 && quadrant(stars[2].point()) == 4) {
    return 0;
  }

  if (quadrant(stars[2].point()) == 3 && quadrant(stars[3].point()) == 3) {
    if (angles[3] > angles[2]) { return 0; }
  }
  if (quadrant(stars[2].point()) == 2 && quadrant(stars[3].point()) == 2) {
    if (angles[3] > angles[2]) { return 0; }
  }
  if (quadrant(stars[2].point()) == 2 && quadrant(stars[3].point()) == 3) {
    return 0;
  }

  for (int i=0; i<stars.size(); i++) {
    for (int j=0; j<stars.size(); j++) {
      if (i == j) { continue; }

      if (stars[i].x == stars[j].x && stars[i].y == stars[j].y) {
        return 0;
      }
    }
  }
  
  double R_1 = stars[0].r;
  double R_2 = stars[1].r;
  double R_3 = stars[2].r;
  double R_4 = stars[3].r;

  if ( ((R_1 > G) || (R_2 > W) || (R_3 > W) || (R_4 > W))
       && ((R_1 > W) || (R_2 > G) || (R_3 > W) || (R_4 > W))
       && ((R_1 > W) || (R_2 > W) || (R_3 > G) || (R_4 > W))
       && ((R_1 > W) || (R_2 > W) || (R_3 > W) || (R_4 > G)) ) {
    return 0;
  }

  return 1;
}

void StarGroup::print() {
    for ( Star s : stars ) {
        s.print();
    }
    std::cout << endl;
}
