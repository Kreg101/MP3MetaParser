#include "Frame.h"

const unsigned char FLAGS_AMOUNT = 4;

class Tag {

    std::vector<Frame*> frames;
    std::string file_name;
    unsigned int version;
    unsigned int revision_number;
    unsigned int size;
    bool flags[FLAGS_AMOUNT];

    void SetVersion(unsigned char version_byte);

    void SetRevisionNumber(unsigned char revision_number_byte);

    void SetFlags(char byte);

    void SetSize(const char* size_bytes);

public:

    Tag(): version(0), revision_number(0), size(0), flags{false, false, false, false} {};

    void SetMeta(const char*, const char*);

    void Add(Frame& frame);

    void OutputInfo() const;

    void OutputFrames() const;

    [[nodiscard]] std::string GetName() const;

    [[nodiscard]] unsigned int GetSize() const;

    [[nodiscard]] std::vector<std::string> GetFramesName() const;

    [[nodiscard]] Frame* GetFrame(const std::string& frame_name) const;
};