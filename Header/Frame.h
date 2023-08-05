#include <bitset>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

const unsigned char FRAME_ID_SIZE = 4;
const unsigned char FRAME_FLAGS_SIZE = 8;
const unsigned char MAX_SIZE_LENGTH = 4;
const unsigned char BITS_IN_BYTE = 8;
const unsigned char LANGUAGES_BYTES_COUNT = 3;
const unsigned char VOLUME_ADJUSTMENT_SIZE = 2;
const unsigned char FREQUENCY_SIZE = 2;
const unsigned char DATE_FORMAT_LENGTH = 8;
const unsigned char SEEK_FRAME_SIZE = 4;

class Frame {
protected:
    char id[FRAME_ID_SIZE];
    int size;
    bool flags[FRAME_FLAGS_SIZE];

public:
    Frame(): id{'0', '0', '0', '0'}, size(0), flags{false, false, false, false, false, false, false} {};

    explicit Frame(const char*);

    virtual void print() const;

    virtual void SetBody(const char*) = 0;

    void SetMeta(const char*);

    void PrintName() const;

    [[nodiscard]] std::string GetName() const;

    [[nodiscard]] int GetSize() const;

    //~Frame() = default;
};


class TextFrame: public Frame {
protected:
    unsigned char encoding;
    std::string info;

public:
    explicit TextFrame(const char* id): Frame(id) {
        encoding = 0;
    };

    void print() const override;

    void SetBody(const char*) override;
};


class UserTextFrame: public TextFrame {

    std::string description;

public:
    explicit UserTextFrame(const char* id): TextFrame(id) {} ;

    void print() const override;

    void SetBody(const char*) override;

};


class LinkFrame: public Frame {

protected:
    std::string text;

public:
    explicit LinkFrame(const char* id): Frame(id) {};

    void print() const override;

    void SetBody(const char*) override;

};


class UserLinkFrame: public LinkFrame {
private:

    unsigned char encoding;
    std::string description;

public:
    explicit UserLinkFrame(const char* id): LinkFrame(id) {
        encoding = 0;
    };

    void print() const override;

    void SetBody(const char*) override;
};


class UniqueFileIdentifierFrame: public Frame {
    std::string owner_identifier;
    std::string identifier;

public:
    explicit UniqueFileIdentifierFrame(const char* id): Frame(id) {};

    void print() const override;

    void SetBody(const char *) override;
};


class UnknownFrame: public Frame {
public:
    explicit UnknownFrame(const char* id): Frame(id) {};

    void SetBody(const char*) override;
};


class EventTimingCodesFrame: public Frame {
    unsigned char time_stamp_format;
    unsigned char type_of_event;
    std::string time_stamp;

public:
    explicit EventTimingCodesFrame(const char* id): Frame(id), time_stamp_format(0), type_of_event(0) {}

    void print() const override;

    void SetBody(const char *) override;
};


class UnsynchronizedLyricsTextFrame: public Frame{
    unsigned char encoding;
    char language[LANGUAGES_BYTES_COUNT];
    std::string content_descriptor;
    std::string text;

public:
    explicit UnsynchronizedLyricsTextFrame(const char* id): Frame(id), encoding(0), language{'0', '0', '0'} {}

    void print() const override;

    void SetBody(const char *) override;

};


class SynchronizedLyricsTextFrame: public Frame {
    unsigned char encoding;
    char language[LANGUAGES_BYTES_COUNT];
    unsigned char time_stamp_format;
    unsigned char content_type;
    std::string content_descriptor;
    std::vector<std::pair<std::string, std::string>> text;

public:
    explicit SynchronizedLyricsTextFrame(const char* id): Frame(id), encoding(0), language{'0', '0', '0'},
    time_stamp_format(0), content_type(0) {}

    void print() const override;

    void SetBody(const char *) override;
};


class CommentFrame: public Frame {
    unsigned char encoding;
    char language[LANGUAGES_BYTES_COUNT];
    std::string content_descriptor;
    std::string text;

public:
    explicit CommentFrame(const char* id): Frame(id), encoding(0), language{'0', '0', '0'} {}

    void print() const override;

    void SetBody(const char *) override;
};


class RelativeVolumeAdjustmentFrame: public Frame {
    std::string identification;
    unsigned char type_of_channel;
    unsigned char volume_adjustment[VOLUME_ADJUSTMENT_SIZE];
    unsigned char bits_representing_peak;
    std::string peak_volume;

public:
    explicit RelativeVolumeAdjustmentFrame(const char* id): Frame(id), type_of_channel(0), volume_adjustment{'0', '0'}, bits_representing_peak(0) {}

    void print() const override;

    void SetBody(const char *) override;
};


class EqualisationFrame: public Frame {
    unsigned char interpolation_method;
    std::string identification;
    char frequency[FREQUENCY_SIZE];
    char volume_adjustment[VOLUME_ADJUSTMENT_SIZE];

public:
    explicit EqualisationFrame(const char* id): Frame(id), interpolation_method(0), frequency{'0', '0'}, volume_adjustment{'0', '0'} {}

    void print() const override;

    void SetBody(const char *) override;
};


class PlayCounterFrame: public Frame {
    std::string counter;

public:
    explicit PlayCounterFrame(const char* id): Frame(id) {}

    void print() const override;

    void SetBody(const char *) override;
};


class PopularimeterFrame: public Frame {
    std::string email;
    unsigned char rating;
    std::string counter;

public:
    explicit PopularimeterFrame(const char* id): Frame(id), rating(0) {}

    void print() const override;

    void SetBody(const char *) override;
};


class RecommendedBufferSizeFrame: public Frame {
    int buffer_size;
    bool embedded_flag;
    int offset_to_next_tag;

public:
    explicit RecommendedBufferSizeFrame(const char* id): Frame(id), buffer_size(0), embedded_flag(false), offset_to_next_tag(0) {}

    void print() const override;

    void SetBody(const char *) override;

};


class LinkedInformationFrame: public Frame {
    unsigned char frame_identifier[FRAME_ID_SIZE];
    std::string url;
    std::string additional_data;

public:
    explicit LinkedInformationFrame(const char* id): Frame(id), frame_identifier{'0', '0', '0', '0'} {}

    void print() const override;

    void SetBody(const char *) override;


};


class PositionalSynchronizationFrame: public Frame {
    unsigned char stamp_format;
    unsigned int position;

public:
    explicit PositionalSynchronizationFrame(const char* id): Frame(id), stamp_format(0), position(0) {}

    void print() const override;

    void SetBody(const char *) override;
};


class TermsOfUseFrame: public Frame {
    unsigned char encoding;
    char language[LANGUAGES_BYTES_COUNT];
    std::string text;

public:
    explicit TermsOfUseFrame(const char* id): Frame(id), encoding(0), language{'0', '0', '0'} {}

    void print() const override;

    void SetBody(const char *) override;
};


class OwnershipFrame: public Frame {
    unsigned char encoding;
    std::string price_paid;
    std::string date_of_burch;
    std::string seller;

public:
    explicit OwnershipFrame(const char* id): Frame(id), encoding(0) {}

    void print() const override;

    void SetBody(const char *) override;
};


class CommercialFrame: public Frame {
    unsigned char encoding;
    std::string price;
    std::string valid_until;
    std::string contact_url;
    unsigned char received_as;
    std::string seller;
    std::string description;
    std::string picture_type;
    std::string seller_logo;

public:
    explicit CommercialFrame(const char* id): Frame(id), encoding(0), received_as(0) {}

    void print() const override;

    void SetBody(const char *) override;
};


class EncryptionMethodRegistrationFrame: public Frame {
    std::string owner_id;
    unsigned char method_symbol;
    std::string encryption_data;

public:
    explicit EncryptionMethodRegistrationFrame(const char* id): Frame(id), method_symbol(0) {}

    void print() const override;

    void SetBody(const char *) override;
};


class GroupIdentificationRegistrationFrame: public Frame {
    std::string owner_id;
    unsigned char group_symbol;
    std::string group_data;

public:
    explicit GroupIdentificationRegistrationFrame(const char* id): Frame(id), group_symbol(0) {}

    void print() const override;

    void SetBody(const char *) override;
};


class PrivateFrame: public Frame {
    std::string owner_id;
    std::string private_data;

public:
    explicit PrivateFrame(const char* id): Frame(id) {}

    void print() const override;

    void SetBody(const char *) override;
};


class SeekFrame: public Frame {
    unsigned int min_offset;

public:
    explicit SeekFrame(const char* id): Frame(id), min_offset(0) {}

    void print() const override;

    void SetBody(const char *) override;
};


