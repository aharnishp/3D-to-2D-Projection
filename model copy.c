
#define numVertices 3
#define numEdges 3
#define numFaces 1

float vertices[numVertices][3] = {
    { 0.0,  0.5, 0.0 },  
    { 0.5, -0.5, 0.0 },  // t
    { -0.5, -0.5, 0.0 }  // 
};


int edges[numEdges][2] = {
    { 0, 1 },
    { 1, 2 },
    { 2, 0 }
};


int faces[numFaces][3] = {
    { 0, 1, 2 }
};