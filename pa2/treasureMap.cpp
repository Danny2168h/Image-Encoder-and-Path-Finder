#include "treasureMap.h"
#include "queue.h"
using namespace std;

treasureMap::treasureMap(const PNG & baseim, const PNG & mazeim, pair<int,int> s)
{
base = baseim;
maze = mazeim;
start = s;
}

void treasureMap::setGrey(PNG & im, pair<int,int> loc){

RGBAPixel * pixel = im.getPixel(loc.first, loc.second);
pixel->r = 2*(pixel->r/4);
pixel->g = 2*(pixel->g/4);
pixel->b = 2*(pixel->b/4);
}

void treasureMap::setLOB(PNG & im, pair<int,int> loc, int d){

RGBAPixel * pixel = im.getPixel(loc.first, loc.second);

unsigned char b = (0b00000011 & d);
unsigned char g = ((d & 0b00001100) >> 2);
unsigned char r = ((d & 0b00110000) >> 4);

int differenceforB = b - (0b00000011 & pixel->b);
int differenceforG = g - (0b00000011 & pixel->g);
int differenceforR = r - (0b00000011 & pixel->r);

pixel->b = pixel->b + differenceforB;
pixel->g = pixel->g + differenceforG;
pixel->r = pixel->r + differenceforR;


}

PNG treasureMap::renderMap(){

PNG copyOfBase = PNG(base);
vector<vector<bool>> visited;
vector<vector<int>> shortestPath;

visited.resize(copyOfBase.width());
shortestPath.resize(copyOfBase.width());

for(unsigned int i = 0; i < copyOfBase.width(); i++) {
    visited[i].resize(copyOfBase.height());
    shortestPath[i].resize(copyOfBase.height());
  
}

Queue<pair<int, int>> toVisit;

visited[start.first][start.second] = true;
shortestPath[start.first][start.second] = 0;
setLOB(copyOfBase, start, shortestPath[start.first][start.second]);
toVisit.enqueue(start);

pair<int, int> currLoc;

while(!toVisit.isEmpty()) {

    currLoc = toVisit.dequeue();

    vector<pair<int,int>> neighbours = neighbors(currLoc);
 
    for (int i = 0; i < 4; i++) {
        
        if (good(visited, currLoc, neighbours[i])) {
            visited[neighbours[i].first][neighbours[i].second] = true;
            shortestPath[neighbours[i].first][neighbours[i].second] = shortestPath[currLoc.first][currLoc.second] + 1;
            setLOB(copyOfBase, neighbours[i], shortestPath[neighbours[i].first][neighbours[i].second]);
           
            toVisit.enqueue(neighbours[i]);
        }
    }
}
return copyOfBase;
}


PNG treasureMap::renderMaze() {

PNG mazeImage = PNG(base);
setGrey(mazeImage, start);

vector<vector<bool>> visited;
visited.resize(mazeImage.width());
for(unsigned int i = 0; i < mazeImage.width(); i++) {
  
    visited[i].resize(mazeImage.height());
}

Queue<pair<int, int>> toVisit;

toVisit.enqueue(start);
visited[start.first][start.second] = true;

pair<int, int> currLoc;

while(!toVisit.isEmpty()) {
   
    currLoc = toVisit.dequeue();
    vector<pair<int,int>> neighbours = neighbors(currLoc);
    for (int i = 0; i < 4; i++) {
        
        if (good(visited, currLoc, neighbours[i])) {
             visited[neighbours[i].first][neighbours[i].second] = true;
             setGrey(mazeImage, neighbours[i]);
             toVisit.enqueue(neighbours[i]);
        }
    }
}


for (int i = start.first - 3; i <= start.first + 3; i ++) {
     if (0 <= i && i <= mazeImage.width() - 1) {
          for (int j = start.second - 3; j <= start.second + 3; j++) {
               if (0 <= j && j <= mazeImage.height() -1) {
                    RGBAPixel *pixel = mazeImage.getPixel(i,j);
                    pixel -> r = 255;
                    pixel -> g = 0;
                    pixel -> b = 0;
               }
          }
     }
}
return mazeImage;

}

bool treasureMap::good(vector<vector<bool>> & v, pair<int,int> curr, pair<int,int> next){

int width = base.width();
int height = base.height();

if (!(next.first >= 0) || !(next.first < width) || !(next.second < height) || !(next.second >= 0)) {
    return false;
}

RGBAPixel * currPixel = maze.getPixel(curr.first, curr.second);
RGBAPixel * nextPixel = maze.getPixel(next.first, next.second);

return !v[next.first][next.second] && (*currPixel == *nextPixel);
}

vector<pair<int,int>> treasureMap::neighbors(pair<int,int> curr) {

vector<pair<int,int>> combinations;

combinations.push_back(make_pair(curr.first - 1, curr.second));
combinations.push_back(make_pair(curr.first, curr.second + 1));
combinations.push_back(make_pair(curr.first + 1, curr.second));
combinations.push_back(make_pair(curr.first, curr.second - 1));

return combinations;

}

