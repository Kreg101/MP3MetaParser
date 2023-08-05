#include <algorithm>
#include <fstream>
#include "../Header/MP3Parser.h"

const unsigned char TAG_NAME_SIZE = 3;
const unsigned char TAG_HEADER_SIZE = 7;
const unsigned char FRAME_HEADER_SIZE = 6;

bool IsDigitOrBigLetter(int n, const char a[]) {
    for (int i = 0; i < n; ++i) {
        if (!((a[i] > 47 && a[i] < 58) || (a[i] > 64 && a[i] < 91))) {
            return false;
        }
    }
    return true;
}

void CyclicShift(int n, char arr[]) {
    for (int i = 0; i < n - 1; ++i) {
        arr[i] = arr[i + 1];
    }
}

void MP3Parser::FrameHandling(Frame* frame, unsigned int& tag_size, std::ifstream& file_in, bool unknown_frame) {

    char frame_header_buffer[FRAME_HEADER_SIZE];
    file_in.read(frame_header_buffer, FRAME_HEADER_SIZE);
    tag_size += FRAME_HEADER_SIZE;

    frame->SetMeta(frame_header_buffer);

    unsigned int frame_size = frame->GetSize();
    char frame_buffer[frame_size];
    tag_size += frame_size;

    file_in.read(frame_buffer, frame_size);

    frame->SetBody(frame_buffer);

    if (!unknown_frame) {
        tags[tags.size() - 1]->Add(*frame);
    }

}

std::fstream& MP3Parser::Output() const {
    auto* file_out = new std::fstream("textoutput.txt", std::ios::binary);
    for (auto& tag: tags) {
        tag->OutputInfo();
        std::cout << '\n';
        tag->OutputFrames();
    }
}

void MP3Parser::Parse(int argc, char** argv) {

    for (int i = 1; i < argc; ++i) {
        std::string file_name = argv[i];
        std::ifstream file_in(file_name, std::ios::binary);
        if (!file_in.is_open()) {
            std::cout << file_name << " cannot be open!\n";
        }

        char detect_tag[TAG_NAME_SIZE];
        char detect_frame[FRAME_ID_SIZE];
        char tag_header_buffer[TAG_HEADER_SIZE];
        char buffer;
        unsigned int tag_size;
        unsigned int actual_tag_size;

        while (file_in.get(buffer)) {

            CyclicShift(TAG_NAME_SIZE, detect_tag);
            detect_tag[TAG_NAME_SIZE - 1] = buffer;

            if (detect_tag[0] == 'I' && detect_tag[1] == 'D' && detect_tag[2] == '3') {

                tags.emplace_back(new Tag());
                tag_size = 0;
                file_in.read(tag_header_buffer, TAG_HEADER_SIZE);
                tags[tags.size() - 1]->SetMeta(tag_header_buffer, argv[i]);
                actual_tag_size = tags[tags.size() - 1]->GetSize();

                while (tag_size < actual_tag_size) {

                    if (tag_size + FRAME_ID_SIZE <= actual_tag_size) {
                        file_in.read(detect_frame, FRAME_ID_SIZE);
                    } else {
                        break;
                    }

                    tag_size += FRAME_ID_SIZE;

                    Frame* frame;
                    bool unknown_frame = false;
                    std::string frame_name(detect_frame, FRAME_ID_SIZE);

                    if (detect_frame[0] == 'T' && IsDigitOrBigLetter(FRAME_ID_SIZE - 1, detect_frame)) {
                        if (frame_name == "TXXX") {
                            frame = new UserTextFrame(detect_frame);
                        } else {
                            frame = new TextFrame(detect_frame);
                        }
                    } else if (detect_frame[0] == 'W' && IsDigitOrBigLetter(FRAME_ID_SIZE - 1, detect_frame)) {
                        if (frame_name == "WXXX") {
                            frame = new UserLinkFrame(detect_frame);
                        } else {
                            frame = new LinkFrame(detect_frame);
                        }
                    } else if (frame_name == "UFID") {
                        frame = new UniqueFileIdentifierFrame(detect_frame);
                    } else if (frame_name == "ETCO") {
                        frame = new EventTimingCodesFrame(detect_frame);
                    } else if (frame_name == "USLT") {
                        frame = new UnsynchronizedLyricsTextFrame(detect_frame);
                    } else if (frame_name == "SYLT") {
                        frame = new SynchronizedLyricsTextFrame(detect_frame);
                    } else if (frame_name == "COMM") {
                        frame = new CommentFrame(detect_frame);
                    } else if (frame_name == "RVA2") {
                        frame = new RelativeVolumeAdjustmentFrame(detect_frame);
                    } else if (frame_name == "EQU2") {
                        frame = new EqualisationFrame(detect_frame);
                    } else if (frame_name == "PCNT") {
                        frame = new PlayCounterFrame(detect_frame);
                    } else if (frame_name == "POPM") {
                        frame = new PopularimeterFrame(detect_frame);
                    } else if (frame_name == "RBUF") {
                        frame = new RecommendedBufferSizeFrame(detect_frame);
                    } else if (frame_name == "LINK") {
                        frame = new LinkedInformationFrame(detect_frame);
                    } else if (frame_name == "POSS") {
                        frame = new PositionalSynchronizationFrame(detect_frame);
                    } else if (frame_name == "USER") {
                        frame = new TermsOfUseFrame(detect_frame);
                    } else if (frame_name == "OWNE") {
                        frame = new OwnershipFrame(detect_frame);
                    } else if (frame_name == "COMR") {
                        frame = new CommercialFrame(detect_frame);
                    } else if (frame_name == "ENCR") {
                        frame = new EncryptionMethodRegistrationFrame(detect_frame);
                    } else if (frame_name == "GRID") {
                        frame = new GroupIdentificationRegistrationFrame(detect_frame);
                    } else if (frame_name == "PRIV") {
                        frame = new PrivateFrame(detect_frame);
                    } else if (frame_name == "SEEK") {
                        frame = new SeekFrame(detect_frame);
                    } else {
                        unknown_frame = true;
                        frame = new UnknownFrame(detect_frame);
                    }
                    FrameHandling(frame, tag_size, file_in, unknown_frame);

                }
                break;
            }
        }
    }
}

std::fstream& MP3Parser::TableOutput() {
    auto* file_out = new std::fstream("output.csv", std::ios::binary);
    for (auto& tag: tags) {
        *file_out << tag->GetName() << ',';
        for (auto& name: tag->GetFramesName()) {
            *file_out << name << ' ';
        }
        *file_out << ",\n";
    }
    return *file_out;
}

Tag* MP3Parser::GetTag(const std::string& tag_name) const {
    for (auto& tag: tags) {
        if (tag->GetName() == tag_name) {
            return tag;
        }
    }
    return nullptr;
}




