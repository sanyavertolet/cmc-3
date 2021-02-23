STYPE
bit_reverse(STYPE value) {
    UTYPE uvalue = (UTYPE) value;
    UTYPE reversed = 0;
    for(int i = 0; i < sizeof(uvalue) * 8; i++) {
        reversed <<= 1;
        reversed |= uvalue & 1;
        uvalue >>= 1;
    }
    return (STYPE) reversed;
}
