// File for shaders

// Shader interface
typedef struct {
    void (*create)();
    void (*compileShader)();
    void (*linkProgram)();
    void (*use)();
    void (*setUniform)();
} ShaderInterface;
