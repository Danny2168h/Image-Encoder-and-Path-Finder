#include "decoder.h"
#include "queue.h"
#include "stack.h"
using namespace std;

decoder::decoder(const PNG & tm, pair<int,int> s)
   :start(s),mapImg(tm) {

vector<vector<bool>> visited;
vector<vector<int>> shortestPath;
vector<vector<pair<int, int>>> discovered;

visited.resize(mapImg.width());
shortestPath.resize(mapImg.width());
discovered.resize(mapImg.width()); 

for(unsigned int i = 0; i < mapImg.width(); i++) {
    visited[i].resize(mapImg.height());
    shortestPath[i].resize(mapImg.height());
    discovered[i].resize(mapImg.height());
}

Queue<pair<int, int>> toVisit;

visited[start.first][start.second] = true;
shortestPath[start.first][start.second] = 0;
discovered[start.first][start.second] = make_pair(-1, -1);
toVisit.enqueue(start);

pair<int, int> currLoc;

while(!toVisit.isEmpty()) {

    currLoc = toVisit.dequeue();

    vector<pair<int,int>> neighbours = neighbors(currLoc);

    for (int i = 0; i < 4; i++) {

        if (good(visited, shortestPath, currLoc, neighbours[i])) {
            visited[neighbours[i].first][neighbours[i].second] = true;
            shortestPath[neighbours[i].first][neighbours[i].second] = shortestPath[currLoc.first][currLoc.second] + 1;
            discovered[neighbours[i].first][neighbours[i].second] = make_pair(currLoc.first, currLoc.second);
            toVisit.enqueue(neighbours[i]);
        }
    }
}


int greatest = 0;
int xCoord = -1;
int yCoord = -1;

for (unsigned int x = 0; x < mapImg.width(); x++) {
    for (unsigned int y = 0; y < mapImg.height(); y++) {
        if (shortestPath[x][y] >= greatest) {
            greatest = shortestPath[x][y];
            xCoord = x;
            yCoord = y;
        }
    }
}


pathPts.resize(greatest + 1);
int iterations = greatest;
pathPts[iterations] = make_pair(xCoord, yCoord);
pair<int, int> curr = discovered[xCoord][yCoord];
while(iterations > 0) {
    iterations--;
    pathPts[iterations] = curr;
    curr = discovered[curr.first][curr.second];
}
}

PNG decoder::renderSolution(){

PNG copyOfMapImg = PNG(mapImg);

for (unsigned int i = 0; i < pathPts.size(); i++) {
   RGBAPixel * curr = copyOfMapImg.getPixel(pathPts[i].first, pathPts[i].second);
   curr->r = 255;
   curr->g = 0;
   curr->b = 0;
}

return copyOfMapImg;
}

PNG decoder::renderMaze(){

PNG copyMapImg = PNG(mapImg);
vector<vector<bool>> visited;
vector<vector<int>> shortestPath;

visited.resize(copyMapImg.width());
shortestPath.resize(copyMapImg.width());

for(unsigned int i = 0; i < copyMapImg.width(); i++) {
    visited[i].resize(copyMapImg.height());
    shortestPath[i].resize(copyMapImg.height());
}

Queue<pair<int, int>> toVisit;

visited[start.first][start.second] = true;
shortestPath[start.first][start.second] = 0;
setGrey(copyMapImg, start);
toVisit.enqueue(start);

pair<int, int> currLoc;


while(!toVisit.isEmpty()) {

   
    currLoc = toVisit.dequeue();
    vector<pair<int,int>> neighbours = neighbors(currLoc);
    for (int i = 0; i < 4; i++) {

        if (good(visited, shortestPath, currLoc, neighbours[i])) {
            visited[neighbours[i].first][neighbours[i].second] = true;
            shortestPath[neighbours[i].first][neighbours[i].second] = shortestPath[currLoc.first][currLoc.second] + 1;
            setGrey(copyMapImg, neighbours[i]);
            toVisit.enqueue(neighbours[i]);
        }
    }
}

for (int i = start.first - 3; i <= start.first + 3; i ++) {
     if (0 <= i && i <= copyMapImg.width() - 1) {
         
          for (int j = start.second - 3; j <= start.second + 3; j++) {
            
               if (0 <= j && i <= copyMapImg.height() -1) {
                    RGBAPixel *pixel = copyMapImg.getPixel(i,j);
                    pixel -> r = 255;
                    pixel -> g = 0;
                    pixel -> b = 0;
               }
          }
     }
}

return copyMapImg;
}

void decoder::setGrey(PNG & im, pair<int,int> loc){

RGBAPixel * pixel = im.getPixel(loc.first, loc.second);
pixel->r = 2*(pixel->r/4);
pixel->g = 2*(pixel->g/4);
pixel->b = 2*(pixel->b/4);
}

pair<int,int> decoder::findSpot(){

return pathPts[pathPts.size()-1];

}

int decoder::pathLength(){

return pathPts.size();

}

bool decoder::good(vector<vector<bool>> & v, vector<vector<int>> & d, pair<int,int> curr, pair<int,int> next){

bool b = (next.first < (int) mapImg.width()) && (next.second < (int) mapImg.height()) && (next.first >= 0 && next.second >= 0);
if (!b) {
    return false;
}

bool a = v[next.first][next.second];
RGBAPixel * p = mapImg.getPixel(next.first, next.second);
bool c = compare(*p, d[curr.first][curr.second]);
return (!a && c);
}

vector<pair<int,int>> decoder::neighbors(pair<int,int> curr) {

vector<pair<int,int>> combinations;

combinations.push_back(make_pair(curr.first - 1, curr.second));
combinations.push_back(make_pair(curr.first, curr.second + 1));
combinations.push_back(make_pair(curr.first + 1, curr.second));
combinations.push_back(make_pair(curr.first, curr.second - 1));

return combinations;

}


bool decoder::compare(RGBAPixel p, int d){

int mazeValue = ((p.r % 4) * 16) + ((p.g % 4) * 4) + (p.b % 4);

return mazeValue == ((d + 1) % 64);
}
