// Texture File

typedef struct {} Texture;

// Texture interface
typedef struct {
    void (*create)();
    void (*load)();
    void (*bind)();
    void (*setParameters)();
    void (*unbind)();
} TextureInterface;
