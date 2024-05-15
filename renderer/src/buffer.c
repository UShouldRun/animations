// Buffer file

typedef struct {} Buffer;

// Buffer interface
typedef struct {
    void (*create)();
    void (*bind)();
    void (*setData)();
    void (*updateData)();
    void (*unbind)();
} BufferInterface;
