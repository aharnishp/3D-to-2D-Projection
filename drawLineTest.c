
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#include "model.h"

#define screenWidth 96
#define screenHeight 128

#define fori(i,n) for(int i = 0; i < n; i++)

#define telemetry 0

struct rotation_s{
    float rx;
    float ry;
    float rz;
};
typedef struct rotation_s rotation;

struct rotation_deg_s{
    int rx;
    int ry;
    int rz;
};
typedef struct rotation_deg_s rotation_deg;

struct camera_s{
    float x;
    float y;
    float z;
    rotation camera_rotation;
};
typedef struct camera_s camera;


float rotated_vertices[numVertices][3];
rotation model_rotation = {0.0,0.0,0.0};



const __uint8_t fast_sine_table[91] = {0, 4, 8, 13, 17, 22, 26, 31, 35, 39, 44, 48, 53, 57, 61, 65, 70, 74, 78, 83, 87, 91, 95, 99, 103, 107, 111, 115, 119, 123, 127, 131, 135, 138, 142, 146, 149, 153, 156, 160, 163, 167, 170, 173, 177, 180, 183, 186, 189, 192, 195, 198, 200, 203, 206, 208, 211, 213, 216, 218, 220, 223, 225, 227, 229, 231, 232, 234, 236, 238, 239, 241, 242, 243, 245, 246, 247, 248, 249, 250, 251, 251, 252, 253, 253, 254, 254, 254, 254, 254, 255};

int fsin(int degree){
    if(degree == 0){ return 0;}
    int answer = 0;
    int needFlip = 0;
    if(degree > 180 || (degree < 0 && degree > -180)){ needFlip = 1; }

    degree = abs(degree%180);
    
    if(degree>=0 && degree <=90){
        answer = fast_sine_table[degree];
    }else if(degree > 90 && degree <= 180){
        answer = fast_sine_table[180-degree];
    }

    if(needFlip){
        answer *= -1;
    }
    return answer;
}

int fcos(int degree){
    return (fsin(90-degree));
}



void rotate_model_from_default(rotation_deg rotationVector){
    // the output is stored in rotated_vertices
    int sinrx = fsin(rotationVector.rx);
    int cosrx = fcos(rotationVector.rx);

    int sinry = fsin(rotationVector.ry);
    int cosry = fcos(rotationVector.ry);

    int sinrz = fsin(rotationVector.rz);
    int cosrz = fcos(rotationVector.rz);

    float axx = (cosrx * cosry)/65025.0;
    float axy = (cosrx * sinry * sinrz)/16581375.0 + (sinrx * cosrz)/65025.0;
    float axz = (cosrx*sinry*cosrz) + (sinrx*sinrz);

    float ayx = (sinrx*cosry);
    float ayy = (sinrx*sinry*sinrz)/16581375.0 + (cosrx*cosrz)/65025.0;
    float ayz = (sinrx*sinry*cosrz)/16581375.0 - (cosrx*sinrz)/65025.0;
    float azx = -sinry/255.0;
    float azy = (cosry*sinrz)/65025.0;
    float azz = (cosry*cosrz)/65025.0;


    fori(v_id, numVertices){
        float x = vertices[v_id][0]; 
        float y = vertices[v_id][1]; 
        float z = vertices[v_id][2]; 

        rotated_vertices[v_id][0] = axx*x + axy*y + axz*z;
        rotated_vertices[v_id][1] = ayx*x + ayy*y + ayz*z;
        rotated_vertices[v_id][2] = azx*x + azy*y + azz*z;
    }
}


// void rotate_model(rotation_deg rotationVector){
//     // the output is stored in rotated_vertices
//     int sinrx = fsin(rotationVector.rx);
//     int cosrx = fcos(rotationVector.rx);

//     int sinry = fsin(rotationVector.ry);
//     int cosry = fcos(rotationVector.ry);

//     int sinrz = fsin(rotationVector.rz);
//     int cosrz = fcos(rotationVector.rz);

//     float axx = (cosrx * cosry)/65025.0;
//     float axy = (cosrx * sinry * sinrz)/16581375.0 + (sinrx * cosrz)/65025.0;
//     float axz = (cosrx*sinry*cosrz) + (sinrx*sinrz);

//     float ayx = (sinrx*cosry);
//     float ayy = (sinrx*sinry*sinrz)/16581375.0 + (cosrx*cosrz)/65025.0;
//     float ayz = (sinrx*sinry*cosrz)/16581375.0 - (cosrx*sinrz)/65025.0;
//     float azx = -sinry/255.0;
//     float azy = (cosry*sinrz)/65025.0;
//     float azz = (cosry*cosrz)/65025.0;


//     fori(v_id, numVertices){
//         float x = rotated_vertices[v_id][0]; 
//         float y = rotated_vertices[v_id][1]; 
//         float z = rotated_vertices[v_id][2]; 

//         rotated_vertices[v_id][0] = axx*x + axy*y + axz*z;
//         rotated_vertices[v_id][1] = ayx*x + ayy*y + ayz*z;
//         rotated_vertices[v_id][2] = azx*x + azy*y + azz*z;
//     }
// }



void drawLine(__uint8_t screen[screenHeight][screenWidth], int x1, int y1, int x2, int y2){
    int x = x1;
    int y = y1; 


    float slope;
    if(x1 != x2){
        slope = (y2 - y1)/(x2 - x1+0.0);
    }else{
        slope = __INT_MAX__;
    }

    int xd = 0;  // direction of x end from start
    if(x2 > x1){
        xd = 1;
    }else if(x2 < x1){
        xd = -1;
    }

    int yd = 0;  // direction of y end from start
    if(y2 > y1){
        yd = 1;
    }else if(y2 < y1){
        yd = -1;
    }


    for(int i = 0; x != x2 || y != y2; i++){
        if(!(x < 0 || x > screenWidth || y < 0 || y > screenHeight)){
            screen[x][y] = 48;
        }
        float lhs = ((x2 - x)*slope);
        if(lhs < 0){ lhs = 0 - lhs; }
        float lhsabs = abs(lhs);
        float rhs = (y2 - y);
        if(rhs < 0){ rhs = 0 - rhs; }
        float rhsabs = abs(rhs);
        if((lhs) >= (rhs)){
            x += xd;
        }else{
            y+= yd;
        }
    }
  

}


void project2D(__uint8_t screen[screenHeight][screenWidth], int axis, float scale, int offset_x, int offset_y){
    fori(edge_id, numEdges){
        int v1 = edges[edge_id][0];
        int v2 = edges[edge_id][1];

        float x1f = rotated_vertices[v1][(0+axis)%3];
        float y1f = rotated_vertices[v1][(1+axis)%3];
        float x2f = rotated_vertices[v2][(0+axis)%3];
        float y2f = rotated_vertices[v2][(1+axis)%3];

        int x1 = scale*x1f + offset_x;
        int y1 = scale*y1f + offset_y;
        int x2 = scale*x2f + offset_x;
        int y2 = scale*y2f + offset_y;

        if(telemetry){ printf("x1=%f y1=%f x2=%f y2=%f\n", x1f,y1f,x2f,y2f); }
        if(telemetry){ printf("x1=%d y1=%d x2=%d y2=%d\n", x1,y1,x2,y2); }

        drawLine(screen, x1, y1, x2, y2);
    }
}

void project_perspective(__uint8_t screen[screenHeight][screenWidth]) {

}


int main(){
    int x1 = 0;
    int y1 = 0;
    int x2 = 48;
    int y2 = 28;
    
    __uint8_t screen[screenHeight][screenWidth];

    camera cam1;
    cam1.x = 1.5;
    cam1.y = 0;
    cam1.z = 0;
    cam1.camera_rotation = (rotation){0.0,0.0,0.0};

    // load model vertices into variable buffer
    rotate_model_from_default((rotation_deg){0,0,0});
    
    // drawLine(screen, x1, y1, x2, y2);

    fori(i,numVertices){
        printf("%f %f %f\n", vertices[i][0],vertices[i][1],vertices[i][2]);
    }
    int rotation_angle = 0;
    while(1){
        printf("Projecting to screen now\n");
        // project2D(screen,2, 95, screenWidth/2, screenHeight/2+0);
//        project2D(screen,1, 15, screenWidth/2, screenHeight/2+0);
        project2D(screen,1, 65, screenWidth/2, screenHeight/2+0); // for pot
        // project2D(screen,1, 5, screenWidth/2, screenHeight/2+0); // for canteen room

        // rotate_model((rotation_deg){4,0,0});
        rotate_model_from_default((rotation_deg){0,rotation_angle/3,rotation_angle/2});
        rotation_angle++;
        if(rotation_angle >= 1080){
            rotation_angle = 0;
        }
        

        // print screen
        fori(j,screenWidth){
            fori(i,screenHeight){
                printf("%c ", screen[i][j]);
            }
            printf("\n");

        }
        

        nanosleep((const struct timespec[]){{0, 10000000L}}, NULL);

        system("clear");
        // empty screen
        fori(i,screenHeight){
            fori(j,screenWidth){
                screen[i][j] = 32;
            }
        }
    }

    printf("Number of faces: %d\n", numFaces);

    return 0;
}
