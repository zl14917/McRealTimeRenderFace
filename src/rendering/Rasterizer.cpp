//
// Created by Zaiyang Li on 19/02/2018.
//

#include "Rasterizer.hpp"
#include "../SDLauxiliary.h"


void McRenderer::Rasterizer::renderToScreen(screen *screen) {

    const int width = screen->width;
    const int height = screen->height;
    memset(screen->buffer, 0, sizeof(uint32_t) * width * height);
    // vec4 point(.5, 0, 1, 1);
    // vec4 point2(-.5, 0, 1, 1);
    //
    // mat4 projMat = scene->camera.projectionMatrix();
    // std::cout << projMat[0][0] << ' ' << projMat[0][1] << ' ' << projMat[0][2] << ' ' << projMat[0][3] << ' '<<'\n'
    //           << projMat[1][0] << ' ' << projMat[1][1] << ' ' << projMat[1][2] << ' ' << projMat[1][3] << ' '<<'\n'
    //           << projMat[2][0] << ' ' << projMat[2][1] << ' ' << projMat[2][2] << ' ' << projMat[2][3] << ' '<<'\n'
    //           << projMat[3][0] << ' ' << projMat[3][1] << ' ' << projMat[3][2] << ' ' << projMat[3][3] << ' '<<'\n'<<endl;

    // mat4 viewProjectionMatrix = scene->camera.viewProjectionMatrix();
    // vec4 transformedPoint = viewProjectionMatrix * point;
    // vec4 transformedPoint2 = viewProjectionMatrix * point2;
    // cout << transformedPoint.x << ' ' << transformedPoint.y << ' ' << transformedPoint.z << ' ' << transformedPoint.w <<endl;
    // int x = (int) (transformedPoint.x/transformedPoint.w * width / 2 + width / 2);
    // int y = (int) (transformedPoint.y/transformedPoint.w * height / 2 + height / 2);
    // int x2 = (int) (transformedPoint2.x/transformedPoint2.w * width / 2 + width / 2);
    // int y2 = (int) (transformedPoint2.y/transformedPoint2.w * height / 2 + height / 2);
    // cout << x << ' ' << y << endl;
    //
    // if(x >= 0 && x < width && y >= 0 && y < height && transformedPoint.z >= 0) {
    //     PutPixelSDL(screen, x, y, vec3(1.0f));
    // }
    // if(x2 >= 0 && x2 < width && y2 >= 0 && y2 < height && transformedPoint.z >= 0) {
    //     PutPixelSDL(screen, x2, y2, vec3(1.0f));
    // }

    Line line = Line({.2, 0.2, 1, 1}, {-.2, 0, 1, 1});
    // vec4 point(-.5, .2, -1, 1);

    mat4 viewProjectionMatrix = scene->camera.viewProjectionMatrix();
    Line projLine = line.projectLine(viewProjectionMatrix);

    std::cout << projLine.ends[0].x << ' '<< projLine.ends[0].y <<' '<< projLine.ends[0].z <<' '<< projLine.ends[0].w <<' '<<'\n';
    std::cout << projLine.ends[1].x << ' '<< projLine.ends[1].y <<' '<< projLine.ends[1].z <<' '<< projLine.ends[1].w <<' '<<'\n';

    clipLine(&projLine);

    int x1 = (int) (projLine.ends[0].x * width / 2 + width / 2);
    int y1 = (int) (projLine.ends[0].y * height / 2 + height / 2);
    int x2 = (int) (projLine.ends[1].x * width / 2 + width / 2);
    int y2 = (int) (projLine.ends[1].y * height / 2 + height / 2);


    std::cout << projLine.ends[0].x << ' '<< projLine.ends[0].y <<' '<< projLine.ends[0].z <<' '<< projLine.ends[0].w <<' '<<'\n';
    std::cout << projLine.ends[1].x << ' '<< projLine.ends[1].y <<' '<< projLine.ends[1].z <<' '<< projLine.ends[1].w <<' '<<'\n';

    bhm_line(screen, x1, y1, x2, y2);

    // if(x1 >= 0 && x1 < width && y1 >= 0 && y1 < height) {
    //     PutPixelSDL(screen, x1, y1, vec3(1.0f));
    // }
    //
    // if(x1 >= 0 && x1 < width && y1 >= 0 && y1 < height) {
    //     PutPixelSDL(screen, x2, y2, vec3(1.0f));
    // }

    }


/**
 *  Compute the intersectin between a line and an axis-aligned plane.
 *  That mean a plane that can be described with the equation `x=x_p` for example.
 *  vec3 plane represent this plane. Only one of the 3 componment must be non-0.
 *  for example, if plane is {0,4,0} it describes the plane y=4.

 * if plane have more than one non-null i have no idea of what the result will be like.

 * @return the coordinates of the intersection. If no intersection is found, it return vec4(0)
 */

vec4 lineAligndPlaneIntersection(Line line, vec3 plane){
  vec4 intersect;
  intersect.w=1;
  vec4 lineDirection = line.ends[1] - line.ends[0];
  if (plane.x != 0){
    if (lineDirection.x == 0){return vec4(0);}
    intersect.x = plane.x;
    intersect.y = line.ends[0].y + lineDirection.y / lineDirection.x * (intersect.x - line.ends[0].x);
    intersect.z = line.ends[0].z + lineDirection.z / lineDirection.x * (intersect.x - line.ends[0].x);
    return intersect;
  }
  else if (plane.y != 0){
    if (lineDirection.y == 0){return vec4(0);}
    intersect.y = plane.y;
    intersect.x = line.ends[0].x + lineDirection.x / lineDirection.y * (intersect.y - line.ends[0].y);
    intersect.z = line.ends[0].z + lineDirection.z / lineDirection.y * (intersect.y - line.ends[0].y);
    return intersect;
  }
  else if (plane.y != 0){
    if (lineDirection.z == 0){return vec4(0);}
    intersect.z = plane.z;
    intersect.x = line.ends[0].x + lineDirection.x / lineDirection.z * (intersect.z - line.ends[0].z);
    intersect.y = line.ends[0].y + lineDirection.y / lineDirection.z * (intersect.z - line.ends[0].z);
    return intersect;
  }
  return vec4(0);
}

void clipLine(Line *line){
    if ((line->ends[0].x < -1 || line->ends[0].x > 1
     || line->ends[0].y < -1 || line->ends[0].y > 1
     || line->ends[0].z < -1 || line->ends[0].z > 1)
     && (line->ends[1].x < -1 || line->ends[1].x > 1
     || line->ends[1].y < -1 || line->ends[1].y > 1
     || line->ends[1].z < -1 || line->ends[1].z > 1)) {
       // Both edges are outside the render cube

       vec4 intersect1 = vec4(0); //if no intersection is found then intersect1.w will be 0.
       vec4 intersect2 = vec4(0);

       vec3 planes[] = {{-1, 0, 0}, {1,0,0},
                        { 0,-1, 0}, {0,1,0},
                        { 0, 0,-1}, {0,0,1}};

        for (size_t i = 0; i < sizeof(planes)/sizeof(planes[0]); i++) {
          vec4 inter = lineAligndPlaneIntersection(*line, planes[i]);
          if (inter.w && inter.x >= -1 && inter.y <= 1
                      && inter.y >= -1 && inter.y <= 1
                      && inter.z >= -1 && inter.z <= 1) { // intersect found in-bounds
                        std::cout << i << '\n';
            if (!intersect1.w){intersect1=inter;}
            else if (inter != intersect1){intersect2=inter;}
          }
        }

        if (intersect1.w && intersect2.w){ // newline is between the 2 found intersections
          line->ends[0] = intersect1;
          line->ends[1] = intersect2;
        }


    }
    else if (line->ends[0].x < -1 || line->ends[0].x > 1
     || line->ends[0].y < -1 || line->ends[0].y > 1
     || line->ends[0].z < -1 || line->ends[0].z > 1){
       // end0 is outside of the rendering cube, end1 inside
       std::cout << "/* message */" << '\n';
       vec3 planes[] = {{-1, 0, 0}, {1,0,0},
                        { 0,-1, 0}, {0,1,0},
                        { 0, 0,-1}, {0,0,1}};

      vec3 minCoord = {line->ends[0].x<line->ends[1].x?line->ends[0].x:line->ends[1].x,
                       line->ends[0].y<line->ends[1].y?line->ends[0].y:line->ends[1].y,
                       line->ends[0].z<line->ends[1].z?line->ends[0].z:line->ends[1].z};
      vec3 maxCoord = {line->ends[0].x>line->ends[1].x?line->ends[0].x:line->ends[1].x,
                       line->ends[0].y>line->ends[1].y?line->ends[0].y:line->ends[1].y,
                       line->ends[0].z>line->ends[1].z?line->ends[0].z:line->ends[1].z};


      for (size_t i = 0; i < sizeof(planes)/sizeof(planes[0]); i++) {
        vec4 inter = lineAligndPlaneIntersection(*line, planes[i]);

        if (inter.w && inter.x > minCoord.x && inter.x < maxCoord.x
                    && inter.y > minCoord.y && inter.y < maxCoord.y
                    && inter.z > minCoord.z && inter.z < maxCoord.z){
                      line->ends[0]=inter;
                    }
      }
     }
     else if (line->ends[1].x < -1 || line->ends[1].x > 1
     || line->ends[1].y < -1 || line->ends[1].y > 1
     || line->ends[1].z < -1 || line->ends[1].z > 1){
       // end1 is outside of rendering cube, end0 inside
       vec3 planes[] = {{-1, 0, 0}, {1,0,0},
                        { 0,-1, 0}, {0,1,0},
                        { 0, 0,-1}, {0,0,1}};

      vec3 minCoord = {line->ends[0].x<line->ends[1].x?line->ends[0].x:line->ends[1].x,
                       line->ends[0].y<line->ends[1].y?line->ends[0].y:line->ends[1].y,
                       line->ends[0].z<line->ends[1].z?line->ends[0].z:line->ends[1].z};
      vec3 maxCoord = {line->ends[0].x>line->ends[1].x?line->ends[0].x:line->ends[1].x,
                       line->ends[0].y>line->ends[1].y?line->ends[0].y:line->ends[1].y,
                       line->ends[0].z>line->ends[1].z?line->ends[0].z:line->ends[1].z};

      for (size_t i = 0; i < sizeof(planes)/sizeof(planes[0]); i++) {
        vec4 inter = lineAligndPlaneIntersection(*line, planes[i]);

        if (inter.w && inter.x > minCoord.x && inter.x < maxCoord.x
                    && inter.y > minCoord.y && inter.y < maxCoord.y
                    && inter.z > minCoord.z && inter.z < maxCoord.z){
                      line->ends[1]=inter;
                    }
      }

     }
}


 /**
  * Bresenham algorithm implementation.
  * Working code from http://www.etechplanet.com/codesnippets/computer-graphics-draw-a-line-using-bresenham-algorithm.aspx.
  * Mine is not working. But it's 2:30AM. I'll fix it later, i promise.
  */

void bhm_line(screen *screen, int x1,int y1,int x2,int y2)
{
 int x,y,dx,dy,dx1,dy1,px,py,xe,ye,i;
 dx=x2-x1;
 dy=y2-y1;
 dx1=fabs(dx);
 dy1=fabs(dy);
 px=2*dy1-dx1;
 py=2*dx1-dy1;
 if(dy1<=dx1)
 {
  if(dx>=0)
  {
   x=x1;
   y=y1;
   xe=x2;
  }
  else
  {
   x=x2;
   y=y2;
   xe=x1;
  }
  PutPixelSDL(screen, x, y, vec3(1.0f));
  for(i=0;x<xe;i++)
  {
   x=x+1;
   if(px<0)
   {
    px=px+2*dy1;
   }
   else
   {
    if((dx<0 && dy<0) || (dx>0 && dy>0))
    {
     y=y+1;
    }
    else
    {
     y=y-1;
    }
    px=px+2*(dy1-dx1);
   }
   PutPixelSDL(screen, x, y, vec3(1.0f));
  }
 }
 else
 {
  if(dy>=0)
  {
   x=x1;
   y=y1;
   ye=y2;
  }
  else
  {
   x=x2;
   y=y2;
   ye=y1;
  }
  PutPixelSDL(screen, x, y, vec3(1.0f));
  for(i=0;y<ye;i++)
  {
   y=y+1;
   if(py<=0)
   {
    py=py+2*dx1;
   }
   else
   {
    if((dx<0 && dy<0) || (dx>0 && dy>0))
    {
     x=x+1;
    }
    else
    {
     x=x-1;
    }
    py=py+2*(dx1-dy1);
   }
   PutPixelSDL(screen, x, y, vec3(1.0f));
  }
 }
}



/**
 * Bresenham's line algorithm
 * https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
 * TODO : Find the error in the algorithm.
 */
    void drawLine(screen *screen, int x1, int y1, int x2, int y2) {

        int dx = x2 - x1;
        int dy = y2 - y1;

        // in the commentaries next, i'll assume that the (0,0) point of the screen
        // is at the top left. Horizontal axis x aims to the right.
        // Vertical axis y aims to the bottom.
        // if reality is not like this the code will work perfectly good (100% reversible).
        // it's just to describe what each block does.

        if (dy < 0) {
            int tmp = x2;
            x2 = x1;
            x1 = tmp;
            tmp = y2;
            y2 = x1;
            y1 = tmp;
            dx = x2 - x1;
            dy = y2 - y1;
        }

        // now dy >= 0 => every line is going down


        if (dx < 0) {
            if (dy > 0) {
                if (-dx >= dy) {
                    // line to the bottom left, close of horizontal
                    int e = dx;
                    dx *= 2;
                    dy *= 2;
                    for (x1; x1 >= x2; x1--) {
                        PutPixelSDL(screen, x1, y1, vec3(1.0f));
                        e += dy;
                        if (e >= 0) {
                            y1++;
                            e += dx;
                        }
                    }

                } else {
                    // line to the bottom left, close of vertical
                    int e = dy;
                    dx *= 2;
                    dy *= 2;
                    for (y1; y1 <= y2; y1++) {
                        PutPixelSDL(screen, x1, y1, vec3(1.0f));
                        e += dx;
                        if (e <= 0) {
                            x1--;
                            e += dy;
                        }
                    }
                }
            } else if (dy == 0) {
                //horizontal line
                for (x1; x1 >= x2; x1--) {
                    PutPixelSDL(screen, x1, y1, vec3(1.0f));
                }
            }
        } else if (dx > 0) {
            if (dy > 0) {
                if (dx >= dy) {
                    // line to the bottom right, close of horizontal
                    int e = dx;
                    dx *= 2;
                    dy *= 2;
                    for (x1; x1 <= x2; x1++) {
                        PutPixelSDL(screen, x1, y1, vec3(1.0f));
                        e = e - dy;
                        if (e < 0) {
                            y1++;
                            e += dx;
                        }
                    }
                } else {
                    // line to the bottom right, close of vertical
                    int e = dy;
                    dx *= 2;
                    dy *= 2;
                    for (y1; y1 <= y2; y1++) {
                        PutPixelSDL(screen, x1, y1, vec3(1.0f));
                        e = e - dx;
                        if (e < 0) {
                            x1++;
                            e += dy;
                        }
                    }
                }
            } else if (dy == 0) {
                // horizontal line
                for (x1; x1 <= x2; x1++) {
                    PutPixelSDL(screen, x1, y1, vec3(1.0f));
                }
            }
        } else if (dx == 0) {
            // vertical line
            for (y1; y1 <= y2; y1++) {
                PutPixelSDL(screen, x1, y1, vec3(1.0f));
            }
        }
    }

    void clipLine3D(const Line &line, Line &result) {

        int outcode1 = computeOutcode3D(line.ends[0], vec4(-1,-1,-1,1), vec4(1,1,1,1));
        int outcode2 = computeOutcode3D(line.ends[1], vec4(-1,-1,-1,1), vec4(1,1,1,1));

        while(true){
            // both end points are inside the region.
            if((outcode1 | outcode2) == INSIDEMASK) {
                break;
            }
        }
    }

    int computeOutcode3D(vec4 point, vec4 min, vec4 max) {
        int outcode = INSIDEMASK;

        if(point.x < min.x) {
            outcode |= LEFTMASK;
        } else if(point.x > max.x) {
            outcode |= RIGHTMASK;
        }

        if(point.y < min.y) {
            outcode |= BOTTOMMASK;
        } else if(point.y > max.y) {
            outcode |= TOPMASK;
        }

        if(point.z < min.z) {
            outcode |= FRONTMASK;
        } else if(point.z > max.z) {
            outcode |= BACKMASK;
        }

        return outcode;
    }

void McRenderer::Rasterizer::fillTriangle(McRenderer::Triangle &triangle, screen *screen) {

}