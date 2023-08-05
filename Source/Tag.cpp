#include "../Header/Tag.h"

void Tag::Add(Frame& frame) {
    frames.emplace_back(&frame);
}

void Tag::OutputInfo() const {
    std::cout << "File name: " << file_name<< "\nID3v2:\nFrames list: ";

    for (auto& frame: frames) {
        frame->PrintName();
    }
    std::cout << "\nVersion: " << version << "\nRevision number: " << revision_number
    << "\nSize: " << size << "\nFlags: ";
    for (auto flag: flags) {
        std::cout << flag;
    }
    std::cout << '\n';
}

void Tag::SetVersion(unsigned char version_byte) {
    version = version_byte;
}

void Tag::SetRevisionNumber(unsigned char revision_number_byte) {
    revision_number = revision_number_byte;
}

void Tag::SetFlags(char byte) {
    auto bits = std::bitset<BITS_IN_BYTE>(byte);
    flags[0] = bits[BITS_IN_BYTE - 1];
    flags[1] = bits[BITS_IN_BYTE - 2];
    flags[2] = bits[BITS_IN_BYTE - 3];
    flags[3] = bits[BITS_IN_BYTE - 4];
}

void Tag::SetSize(const char* size_bytes) {
    std::vector<bool> bits;
    for (int i = 0; i < MAX_SIZE_LENGTH; i++) {
        auto byte = std::bitset<BITS_IN_BYTE>(size_bytes[i]);
        for (int j = BITS_IN_BYTE - 2; j >= 0; --j) {
            bits.emplace_back(byte[j]);
        }
    }
    for (int i = (int)bits.size() - 1; i >= 0; --i) {
        size += bits[i]*(int)pow(2, (double)bits.size() - i - 1);
    }
}

void Tag::SetMeta(const char *meta_in_header, const char* name) {
    SetVersion(*meta_in_header);
    SetRevisionNumber(*(meta_in_header + 1));
    SetFlags(*(meta_in_header + 2));
    SetSize(meta_in_header + 3);
    file_name = name;
}

void Tag::OutputFrames() const {
    for (auto& frame: frames) {
        frame->print();
        //delete frame;
        std::cout << '\n';
    }
}

unsigned int Tag::GetSize() const {
    return size;
}

std::string Tag::GetName() const {
    return file_name;
}

std::vector<std::string> Tag::GetFramesName() const {
    std::vector<std::string> result;
    for (auto& frame: frames) {
        result.emplace_back(frame->GetName());
    }
    return result;
}

Frame* Tag::GetFrame(const std::string& frame_name) const {
    for (auto& frame: frames) {
        if (frame->GetName() == frame_name) {
            return frame;
        }
    }
    return nullptr;
}




