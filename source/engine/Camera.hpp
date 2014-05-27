
class Camera { 
  public:
    float pos[3];
    float rot[3];
    
    Camera(float, float, float, float);
    void getMatrix(float*);
  private:
    float fovy;
    float aspect;
    float zNear;
    float zFar;
};

