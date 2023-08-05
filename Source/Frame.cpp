#include "../Header/Frame.h"

int BytesToSize(const std::string& str) {
    std::vector<bool> bits;
    for (auto s: str) {
        auto byte = std::bitset<BITS_IN_BYTE>(s);
        for (int j = BITS_IN_BYTE - 1; j >= 0; --j) {
            bits.emplace_back(byte[j]);
        }
    }
    int count = 0;
    for (int i = (int)bits.size() - 1; i >= 0; --i) {
        count += bits[i]*(int)pow(2, (double)bits.size() - i - 1);
    }
    return count;
}

Frame::Frame(const char* id): Frame() {
    std::copy(id, id + FRAME_ID_SIZE, this->id);
}

void Frame::PrintName() const{
    for (char i : id) {
        std::cout << i;
    }
    std::cout << ' ';
}

void Frame::SetMeta(const char *meta) {

    std::vector<bool> bits;
    for (int i = 0; i < MAX_SIZE_LENGTH; i++) {
        auto byte = std::bitset<BITS_IN_BYTE>(meta[i]);
        for (int j = BITS_IN_BYTE - 2; j >= 0; --j) {
            bits.emplace_back(byte[j]);
        }
    }
    for (int i = (int)bits.size() - 1; i >= 0; --i) {
        size += bits[i]*(int)pow(2, (double)bits.size() - i - 1);
    }

    auto flag_bits = std::bitset<BITS_IN_BYTE>(*(meta+MAX_SIZE_LENGTH));
    flags[0] = flag_bits[BITS_IN_BYTE - 2];
    flags[1] = flag_bits[BITS_IN_BYTE - 3];
    flags[2] = flag_bits[BITS_IN_BYTE - 4];

    flag_bits = std::bitset<BITS_IN_BYTE>(*(meta + MAX_SIZE_LENGTH + 1));
    flags[3] = flag_bits[BITS_IN_BYTE - 2];
    flags[4] = flag_bits[BITS_IN_BYTE - 5];
    flags[5] = flag_bits[BITS_IN_BYTE - 6];
    flags[6] = flag_bits[BITS_IN_BYTE - 7];
    flags[7] = flag_bits[BITS_IN_BYTE - 8];
}

int Frame::GetSize() const {
    return size;
}

void Frame::print() const {
    for (auto& ch: id) {
        std::cout << ch;
    }
    std::cout << '\n' << "Flags: ";
    for (auto ch: flags) {
        std::cout << ch;
    }
    std::cout << '\n' << "Size: " << size << '\n';
}

std::string Frame::GetName() const {
    std::string a(id, FRAME_ID_SIZE);
    return a;
}

void TextFrame::print() const {
    Frame::print();
    std::cout << "Encoding: " << (unsigned int)encoding << "\nInfo: " << info << '\n';
}

void TextFrame::SetBody(const char* body) {
    int length = 0;
    encoding = *body;
    ++length;
    while (length < size - 1) {
        info += *(body + length);
        ++length;
    }
}

void UserTextFrame::print() const {
    TextFrame::print();
    std::cout << "Description: " << description << '\n';
}

void UserTextFrame::SetBody(const char* body) {
    int length = 0;
    encoding = *body;
    ++length;
    while (*(body + length)) {
        description += *(body + length);
        ++length;
    }
    if (encoding == 1 || encoding == 2) {
        ++length;
    }
    ++length;
    while (length < size - 1) {
        info += *(body + length);
        ++length;
    }
}

void LinkFrame::print() const {
    Frame::print();
    std::cout << "Text: " << text << '\n';
}

void LinkFrame::SetBody(const char* body)  {
    int length = 0;
    while (length < size - 1) {
        text += *(body + length);
        ++length;
    }
}

void UserLinkFrame::print() const {
    LinkFrame::print();
    std::cout << "Encoding: " << (unsigned int)encoding << "\nDescription: " << description << '\n';
}

void UserLinkFrame::SetBody(const char* body) {
    int length = 0;
    encoding = *body;
    ++length;
    while (*(body + length)) {
        description += *(body + length);
        ++length;
    }
    if (encoding == 1 || encoding == 2) {
        ++length;
    }
    ++length;
    while (length < size - 1) {
        text += *(body + length);
        ++length;
    }
}

void UniqueFileIdentifierFrame::print() const {
    Frame::print();
    std::cout << "Owner identifier: " << owner_identifier << "\nIdentifier: " << identifier << '\n';
}

void UniqueFileIdentifierFrame::SetBody(const char* body) {
    int length = 0;
    while (*(body + length)) {
            owner_identifier += *(body + length);
            ++length;
    }
    ++length;
    while (length < size - 1) {
        identifier += *(body + length);
        ++length;
    }
}

void UnknownFrame::SetBody(const char* body) {}

void EventTimingCodesFrame::print() const {
    Frame::print();
    std::cout << "Time stamp format: " << (unsigned int)time_stamp_format << "\nType of event: " << (unsigned int)type_of_event <<
    "\nTime stamp: ";
    for (auto ch: time_stamp) {
        std::cout << (unsigned int)ch << ' ';
    }
    std::cout << '\n';
}

void EventTimingCodesFrame::SetBody(const char* body) {
    int length = 0;
    time_stamp_format = *body;
    ++length;
    type_of_event = *(body + length);
    ++length;
    while (length < size - 1) {
        time_stamp += *(body + length);
        ++length;
    }
}

void UnsynchronizedLyricsTextFrame::print() const {
    Frame::print();
    std::cout << "Encoding: " << (unsigned int)encoding << "\nLanguage: ";
    for (auto ch: language) {
        std::cout << ch;
    }
    std::cout << "\nContent descriptor: " << content_descriptor << "\nText/Lyrics: " << text << '\n';
}

void UnsynchronizedLyricsTextFrame::SetBody(const char* body) {
    int length = 0;
    encoding = *body;
    ++length;
    for (auto& ch: language) {
        ch = *(body + length);
        ++length;
    }
    while (*(body + length)) {
        content_descriptor += *(body + length);
        ++length;
    }
    if (encoding == 1 || encoding == 2) {
        ++length;
    }
    ++length;
    while (length < size - 1) {
        text += *(body + length);
        ++length;
    }
}

void SynchronizedLyricsTextFrame::print() const {
    Frame::print();
    std::cout << "Encoding: " << (unsigned int)encoding << "\nLanguage: ";
    for (auto ch: language) {
        std::cout << ch;
    }
    std::cout << "\nTime stamp format: " << (unsigned int)time_stamp_format << "\nContent type: " << (unsigned int)content_type <<
    "\nContent descriptor: " << content_descriptor << "\nInfo:\n";
    for (auto& pair: text) {
        std::cout << pair.first << ' ';
        for (unsigned char ch: pair.second) {
            std::cout << (unsigned int)ch << ' ';
        }
        std::cout << '\n';
    }
}

void SynchronizedLyricsTextFrame::SetBody(const char* body) {
    int length = 0;
    encoding = *body;
    ++length;
    for (auto& ch: language) {
        ch = *(body + length);
        ++length;
    }
    time_stamp_format = *(body + length);
    ++length;
    content_type = *(body + length);
    ++length;
    while (*(body + length)) {
        content_descriptor += *(body + length);
        ++length;
    }
    if (encoding == 1 || encoding == 2) {
        ++length;
    }
    ++length;
    text.emplace_back(std::pair <std::string, std::string>());

    while (length < size) {
        if (*(body + length)) {
            text[text.size() - 1].first += *(body + length);
            ++length;
        } else {
            if (encoding == 1 || encoding == 2) {
                ++length;
            }
            ++length;
            while (*(body + length) != '\"') {
                text[text.size() - 1].second += *(body + length);
                ++length;
            }
            text.emplace_back(std::pair <std::string, std::string>());
        }
    }
}

void CommentFrame::print() const {
    Frame::print();
    std::cout << "Encoding: " << (unsigned int)encoding << "\nLanguage: ";
    for (auto ch: language) {
        std::cout << ch;
    }
    std::cout << "\nContent descriptor: " << content_descriptor << "\nText: " << text << '\n';
}

void CommentFrame::SetBody(const char* body) {
    int length = 0;
    encoding = *body;
    ++length;
    for (auto& ch: language) {
        ch = *(body + length);
        ++length;
    }
    while (*(body + length)) {
        content_descriptor += *(body + length);
        ++length;
    }
    if (encoding == 1 || encoding == 2) {
        ++length;
    }
    ++length;
    while (length < size) {
        text += *(body + length);
        ++length;
    }
}

void RelativeVolumeAdjustmentFrame::print() const {
    Frame::print();
    std::cout << "Identification: " << identification << "\nType of channel: " << (unsigned int)type_of_channel << "\nVolume adjustment: ";
    for (auto ch: volume_adjustment) {
        std::cout << (unsigned int)ch << ' ';
    }
    std::cout << "\nBits representing peaks: " << (unsigned int)bits_representing_peak << "\nPeak volume: ";
    for (auto ch: peak_volume) {
        std::cout << (unsigned int)ch << ' ';
    }
    std::cout << '\n';
}

void RelativeVolumeAdjustmentFrame::SetBody(const char* body) {
    int length = 0;
    while (*(body + length)) {
        identification += *(body + length);
        ++length;
    }
    ++length;
    type_of_channel = *(body + length);
    ++length;
    for (auto& ch: volume_adjustment) {
        ch = *(body + length);
        ++length;
    }
    bits_representing_peak = *(body + length);
    ++length;
    while (length < size) {
        peak_volume += *(body + length);
        ++length;
    }
}

void EqualisationFrame::print() const {
    Frame::print();
    std::cout << "Interpolation method: " << (unsigned int)interpolation_method << "\nIdentification: " << identification << "\nFrequency: "
    << BytesToSize(std::string(frequency, FREQUENCY_SIZE)) << "\nVolume adjustment: " <<
    BytesToSize(std::string(volume_adjustment, VOLUME_ADJUSTMENT_SIZE)) << '\n';
}

void EqualisationFrame::SetBody(const char* body) {
    int length = 0;
    interpolation_method = *body;
    ++length;
    while (*(body + length)) {
        identification += *(body + length);
        ++length;
    }
    ++length;
    for (auto& ch: frequency) {
        ch = *(body + length);
        ++length;
    }
    for (auto& ch: volume_adjustment) {
        ch = *(body + length);
        ++length;
    }
}

void PlayCounterFrame::print() const {
    Frame::print();
    std::cout << "Counter: " << BytesToSize(counter) << '\n';

}

void PlayCounterFrame::SetBody(const char* body) {
    int length = 0;
    while (length < size) {
        counter += *(body + length);
        ++length;
    }
}

void PopularimeterFrame::print() const {
    Frame::print();
    std::cout << "Email to user: " << email << "\nRating: " << (unsigned int)rating << "\nCounter: " << BytesToSize(counter) << '\n';
}

void PopularimeterFrame::SetBody(const char* body) {
    int length = 0;
    while (*(body + length)) {
        email += *(body + length);
        ++length;
    }
    ++length;
    rating = *(body + length);
    ++length;
    while (length < size) {
        counter += *(body + length);
        ++length;
    }
}

void RecommendedBufferSizeFrame::print() const {
    Frame::print();
    std::cout << "Buffer size: " << buffer_size << "\nEmbedded info flag: " << embedded_flag << "\nOffset to next tag: "
    << offset_to_next_tag << '\n';
}

void RecommendedBufferSizeFrame::SetBody(const char* body) {
    int length = 0;
    std::string str;
    while (length < 3) {
        str += *(body + length);
        ++length;
    }
    buffer_size = BytesToSize(str);
    str.clear();
    if (*(body + length)) {
        embedded_flag = true;
    }
    ++length;
    while (length < size) {
        str += *(body + length);
        ++length;
    }
    offset_to_next_tag = BytesToSize(str);
}

void LinkedInformationFrame::print() const {
    Frame::print();
    std::cout << "Frame identifier: ";
    for (auto ch: frame_identifier) {
        std::cout << ch;
    }
    std::cout << "\nUrl: " << url << "\nId and additional data: " << additional_data << '\n';
}

void LinkedInformationFrame::SetBody(const char* body) {
    int length = 0;
    while (length < FRAME_ID_SIZE) {
        frame_identifier[length] = *(body + length);
        ++length;
    }
    while (*(body + length)) {
        url += *(body + length);
        ++length;
    }
    ++length;
    while (length < size) {
        additional_data += *(body + length);
        ++length;
    }
}

void PositionalSynchronizationFrame::print() const {
    Frame::print();
    std::cout << "Stamp format: " << (unsigned int)stamp_format << "\nPosition: " << position << '\n';
}

void PositionalSynchronizationFrame::SetBody(const char* body) {
    int length = 0;
    stamp_format = *body;
    ++length;
    std::string pos;
    while (length < size) {
        pos += *(body + length);
        ++length;
    }
    position = BytesToSize(pos);
}

void TermsOfUseFrame::print() const {
    Frame::print();
    std::cout << "Encoding: " << (unsigned int)encoding << "\nLanguage: ";
    for (auto ch: language) {
        std::cout << ch;
    }
    std::cout << "\nText: " << text << '\n';
}

void TermsOfUseFrame::SetBody(const char* body) {
    int length = 0;
    encoding = *body;
    ++length;
    for (auto& ch: language) {
        ch = *(body + length);
        ++length;
    }
    while (length < size) {
        text += *(body + length);
        ++length;
    }
}

void OwnershipFrame::print() const {
    Frame::print();
    std::cout << "Encoding: " << (unsigned int)encoding << "\nPrice paid: " << price_paid << "\nData of burch: " <<
    date_of_burch << "\nSeller: " << seller << '\n';
}

void OwnershipFrame::SetBody(const char* body) {
    int length = 0;
    encoding = *body;
    ++length;
    while (*(body + length)) {
        price_paid += *(body + length);
        ++length;
    }
    ++length;
    for (int i = 0; i < DATE_FORMAT_LENGTH; ++i) {
        date_of_burch += *(body + length);
        ++length;
    }
    while (length < size) {
        seller += *(body + length);
        ++length;
    }
}

void CommercialFrame::print() const {
    Frame::print();
    std::cout << "Text encoding: " << (unsigned int)encoding << "\nPrice: " << price << "\nValid until: " << valid_until <<
    "\nReceived as: " << received_as << "\nName of seller: " << seller << "\nDescription: " << description <<
    "\nPicture MIME type: " << picture_type << "\nSeller logo: " << seller_logo << '\n';
}

void CommercialFrame::SetBody(const char* body) {
    int length = 0;
    encoding = *body;
    ++length;
    while (*(body + length)) {
        price += *(body + length);
        ++length;
    }
    ++length;
    for (int i = 0; i < DATE_FORMAT_LENGTH; ++i) {
        valid_until += *(body + length);
        ++length;
    }
    while (*(body + length)) {
        contact_url += *(body + length);
        ++length;
    }
    ++length;
    received_as = *(body + length);
    ++length;
    while (*(body + length)) {
        seller += *(body + length);
        ++length;
    }
    if (encoding == 1 || encoding == 2) {
        ++length;
    }
    ++length;
    while (*(body + length)) {
        description += *(body + length);
        ++length;
    }
    if (encoding == 1 || encoding == 2) {
        ++length;
    }
    ++length;
    if (length < size) {
        while (*(body + length)) {
            picture_type += *(body + length);
            ++length;
        }
        ++length;
        while (length < size) {
            seller_logo += *(body + length);
            ++length;
        }
    }


}

void EncryptionMethodRegistrationFrame::print() const {
    Frame::print();
    std::cout << "Owner identifier: " << owner_id << "\nMethod symbol: " << (unsigned int)method_symbol <<
    "\nEncryption data: ";
    for (auto ch: encryption_data) {
        std::cout << (unsigned int)ch << ' ';
    }
    std::cout << '\n';
}

void EncryptionMethodRegistrationFrame::SetBody(const char* body) {
    int length = 0;
    while (*(body + length)) {
        owner_id += *(body + length);
        ++length;
    }
    ++length;
    method_symbol = *(body + length);
    ++length;
    while (length < size) {
        encryption_data += *(body + length);
        ++length;
    }
}

void GroupIdentificationRegistrationFrame::print() const {
    Frame::print();
    std::cout << "Owner identifier: " << owner_id << "\nGroup symbol: " << (unsigned int)group_symbol <<
    "\nGroup dependent data: ";
    for (auto ch: group_data) {
        std::cout << (unsigned int)ch << ' ';
    }
    std::cout << '\n';
}

void GroupIdentificationRegistrationFrame::SetBody(const char* body) {
    int length = 0;
    while (*(body + length)) {
        owner_id += *(body + length);
        ++length;
    }
    ++length;
    group_symbol = *(body + length);
    ++length;
    while (length < size) {
        group_data += *(body + length);
        ++length;
    }
}

void PrivateFrame::print() const {
    Frame::print();
    std::cout << "Owner identifier: " << owner_id << "\nThe private data: ";
    for (auto ch: private_data) {
        std::cout << (unsigned int)ch << ' ';
    }
    std::cout << '\n';
}

void PrivateFrame::SetBody(const char* body) {
    int length = 0;
    while (*(body + length)) {
        owner_id += *(body + length);
        ++length;
    }
    ++length;
    while (length < size) {
        private_data += *(body + length);
        ++length;
    }
}

void SeekFrame::print() const {
    Frame::print();
    std::cout << "Minimum offset to next tag: " << min_offset << '\n';
}

void SeekFrame::SetBody(const char* body) {
    std::string str;
    for (int i = 0; i < SEEK_FRAME_SIZE; ++i) {
        str += *(body + i);
    }
    min_offset = BytesToSize(str);
}
