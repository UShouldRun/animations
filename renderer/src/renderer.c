// Renderer File

typedef struct {} Renderer;

// Renderer interface
typedef struct {
    void (*initialize)();
    void (*setViewport)();
    void (*clear)();
    void (*drawArrays)();
    void (*drawElements)();
    void (*setClearColor)();
    void (*setDepthTest)();
    void (*setBlendMode)();
    void (*setShader)();
    void (*setTexture)();
} RendererInterface;
