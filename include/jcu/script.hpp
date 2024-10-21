// Copyright © 2018-2021 Muhammad Tayyab Akram

#pragma once


#include <cstdint>


namespace jcu {


/**
 * Constants that specify the script of a character.
 */
enum class Script : uint8_t {
    NIL  = 0x00,

    ZINH = 0x01,    //<  Inherited
    ZYYY = 0x02,    //<  Common
    ZZZZ = 0x03,    //<  Unknown

    /* Unicode 1.1  */
    ARAB = 0x04,    //<  Arabic
    ARMN = 0x05,    //<  Armenian
    BENG = 0x06,    //<  Bengali
    BOPO = 0x07,    //<  Bopomofo
    CYRL = 0x08,    //<  Cyrillic
    DEVA = 0x09,    //<  Devanagari
    GEOR = 0x0A,    //<  Georgian
    GREK = 0x0B,    //<  Greek
    GUJR = 0x0C,    //<  Gujarati
    GURU = 0x0D,    //<  Gurmukhi
    HANG = 0x0E,    //<  Hangul
    HANI = 0x0F,    //<  Han
    HEBR = 0x10,    //<  Hebrew
    HIRA = 0x11,    //<  Hiragana
    KANA = 0x12,    //<  Katakana
    KNDA = 0x13,    //<  Kannada
    LAOO = 0x14,    //<  Lao
    LATN = 0x15,    //<  Latin
    MLYM = 0x16,    //<  Malayalam
    ORYA = 0x17,    //<  Oriya
    TAML = 0x18,    //<  Tamil
    TELU = 0x19,    //<  Telugu
    THAI = 0x1A,    //<  Thai

    /* Unicode 2.0 */
    TIBT = 0x1B,    //<  Tibetan

    /* Unicode 3.0 */
    BRAI = 0x1C,    //<  Braille
    CANS = 0x1D,    //<  Canadian_Aboriginal
    CHER = 0x1E,    //<  Cherokee
    ETHI = 0x1F,    //<  Ethiopic
    KHMR = 0x20,    //<  Khmer
    MONG = 0x21,    //<  Mongolian
    MYMR = 0x22,    //<  Myanmar
    OGAM = 0x23,    //<  Ogham
    RUNR = 0x24,    //<  Runic
    SINH = 0x25,    //<  Sinhala
    SYRC = 0x26,    //<  Syriac
    THAA = 0x27,    //<  Thaana
    YIII = 0x28,    //<  Yi

    /* Unicode 3.1 */
    DSRT = 0x29,    //<  Deseret
    GOTH = 0x2A,    //<  Gothic
    ITAL = 0x2B,    //<  Old_Italic

    /* Unicode 3.2 */
    BUHD = 0x2C,    //<  Buhid
    HANO = 0x2D,    //<  Hanunoo
    TAGB = 0x2E,    //<  Tagbanwa
    TGLG = 0x2F,    //<  Tagalog

    /* Unicode 4.0 */
    CPRT = 0x30,    //<  Cypriot
    LIMB = 0x31,    //<  Limbu
    LINB = 0x32,    //<  Linear_B
    OSMA = 0x33,    //<  Osmanya
    SHAW = 0x34,    //<  Shavian
    TALE = 0x35,    //<  Tai_Le
    UGAR = 0x36,    //<  Ugaritic

    /* Unicode 4.1 */
    BUGI = 0x37,    //<  Buginese
    COPT = 0x38,    //<  Coptic
    GLAG = 0x39,    //<  Glagolitic
    KHAR = 0x3A,    //<  Kharoshthi
    SYLO = 0x3B,    //<  Syloti_Nagri
    TALU = 0x3C,    //<  New_Tai_Lue
    TFNG = 0x3D,    //<  Tifinagh
    XPEO = 0x3E,    //<  Old_Persian

    /* Unicode 5.0 */
    BALI = 0x3F,    //<  Balinese
    NKOO = 0x40,    //<  Nko
    PHAG = 0x41,    //<  Phags_Pa
    PHNX = 0x42,    //<  Phoenician
    XSUX = 0x43,    //<  Cuneiform

    /* Unicode 5.1 */
    CARI = 0x44,    //<  Carian
    CHAM = 0x45,    //<  Cham
    KALI = 0x46,    //<  Kayah_Li
    LEPC = 0x47,    //<  Lepcha
    LYCI = 0x48,    //<  Lycian
    LYDI = 0x49,    //<  Lydian
    OLCK = 0x4A,    //<  Ol_Chiki
    RJNG = 0x4B,    //<  Rejang
    SAUR = 0x4C,    //<  Saurashtra
    SUND = 0x4D,    //<  Sundanese
    VAII = 0x4E,    //<  Vai

    /* Unicode 5.2 */
    ARMI = 0x4F,    //<  Imperial_Aramaic
    AVST = 0x50,    //<  Avestan
    BAMU = 0x51,    //<  Bamum
    EGYP = 0x52,    //<  Egyptian_Hieroglyphs
    JAVA = 0x53,    //<  Javanese
    KTHI = 0x54,    //<  Kaithi
    LANA = 0x55,    //<  Tai_Tham
    LISU = 0x56,    //<  Lisu
    MTEI = 0x57,    //<  Meetei_Mayek
    ORKH = 0x58,    //<  Old_Turkic
    PHLI = 0x59,    //<  Inscriptional_Pahlavi
    PRTI = 0x5A,    //<  Inscriptional_Parthian
    SAMR = 0x5B,    //<  Samaritan
    SARB = 0x5C,    //<  Old_South_Arabian
    TAVT = 0x5D,    //<  Tai_Viet

    /* Unicode 6.0 */
    BATK = 0x5E,    //<  Batak
    BRAH = 0x5F,    //<  Brahmi
    MAND = 0x60,    //<  Mandaic

    /* Unicode 6.1 */
    CAKM = 0x61,    //<  Chakma
    MERC = 0x62,    //<  Meroitic_Cursive
    MERO = 0x63,    //<  Meroitic_Hieroglyphs
    PLRD = 0x64,    //<  Miao
    SHRD = 0x65,    //<  Sharada
    SORA = 0x66,    //<  Sora_Sompeng
    TAKR = 0x67,    //<  Takri

    /* Unicode 7.0 */
    AGHB = 0x68,    //<  Caucasian_Albanian
    BASS = 0x69,    //<  Bassa_Vah
    DUPL = 0x6A,    //<  Duployan
    ELBA = 0x6B,    //<  Elbasan
    GRAN = 0x6C,    //<  Grantha
    HMNG = 0x6D,    //<  Pahawh_Hmong
    KHOJ = 0x6E,    //<  Khojki
    LINA = 0x6F,    //<  Linear_A
    MAHJ = 0x70,    //<  Mahajani
    MANI = 0x71,    //<  Manichaean
    MEND = 0x72,    //<  Mende_Kikakui
    MODI = 0x73,    //<  Modi
    MROO = 0x74,    //<  Mro
    NARB = 0x75,    //<  Old_North_Arabian
    NBAT = 0x76,    //<  Nabataean
    PALM = 0x77,    //<  Palmyrene
    PAUC = 0x78,    //<  Pau_Cin_Hau
    PERM = 0x79,    //<  Old_Permic
    PHLP = 0x7A,    //<  Psalter_Pahlavi
    SIDD = 0x7B,    //<  Siddham
    SIND = 0x7C,    //<  Khudawadi
    TIRH = 0x7D,    //<  Tirhuta
    WARA = 0x7E,    //<  Warang_Citi

    /* Unicode 8.0 */
    AHOM = 0x7F,    //<  Ahom
    HATR = 0x80,    //<  Hatran
    HLUW = 0x81,    //<  Anatolian_Hieroglyphs
    HUNG = 0x82,    //<  Old_Hungarian
    MULT = 0x83,    //<  Multani
    SGNW = 0x84,    //<  SignWriting

    /* Unicode 9.0 */
    ADLM = 0x85,    //<  Adlam
    BHKS = 0x86,    //<  Bhaiksuki
    MARC = 0x87,    //<  Marchen
    NEWA = 0x88,    //<  Newa
    OSGE = 0x89,    //<  Osage
    TANG = 0x8A,    //<  Tangut

    /* Unicode 10.0 */
    GONM = 0x8B,    //<  Masaram_Gondi
    NSHU = 0x8C,    //<  Nushu
    SOYO = 0x8D,    //<  Soyombo
    ZANB = 0x8E,    //<  Zanabazar_Square

    /* Unicode 11.0 */
    DOGR = 0x8F,    //<  Dogra
    GONG = 0x90,    //<  Gunjala_Gondi
    MAKA = 0x91,    //<  Makasar
    MEDF = 0x92,    //<  Medefaidrin
    ROHG = 0x93,    //<  Hanifi_Rohingya
    SOGD = 0x94,    //<  Sogdian
    SOGO = 0x95,    //<  Old_Sogdian

    /* Unicode 12.0 */
    ELYM = 0x96,    //<  Elymaic
    HMNP = 0x97,    //<  Nyiakeng_Puachue_Hmong
    NAND = 0x98,    //<  Nandinagari
    WCHO = 0x99,    //<  Wancho

    /* Unicde 13.0 */
    CHRS = 0x9A,    //<  Chorasmian
    DIAK = 0x9B,    //<  Dives_Akuru
    KITS = 0x9C,    //<  Khitan_Small_Script
    YEZI = 0x9D,    //<  Yezidi

    /* Unicde 14.0 */
    CPMN = 0x9E,    //<  Cypro_Minoan
    OUGR = 0x9F,    //<  Old_Uyghur
    TNSA = 0xA0,    //<  Tangsa
    TOTO = 0xA1,    //<  Toto
    VITH = 0xA2,    //<  Vithkuqi

    /* Unicde 15.1 */
    KAWI = 0xA3,    //<  Kawi
    NAGM = 0xA4,    //<  Nag_Mundari

    /* Unicde 16.0 */
    GARA = 0xA5,    //<  Garay
    GUKH = 0xA6,    //<  Gurung_Khema
    KRAI = 0xA7,    //<  Kirat_Rai
    ONAO = 0xA8,    //<  Ol_Onal
    SUNU = 0xA9,    //<  Sunuwar
    TODR = 0xAA,    //<  Todhri
    TUTG = 0xAB     //<  Tulu_Tigalari
};


/**
 * Returns the OpenType tag of a script as UInt32 in big endian byte order. The association between
 * Unicode Script property and OpenType script tags is taken from the specification:
 * https://docs.microsoft.com/en-us/typography/opentype/spec/scripttags.
 *
 * If more than one tag is associated with a script, then the latest one is retured. For example,
 * Devanagari script has two tags, `deva` and `dev2`. So in this case, `dev2` will be returned.
 *
 * If no tag is associated with a script, then `DFLT` is returned.
 *
 * @param script
 *      The script whose OpenType tag is returned.
 * @return
 *      The OpenType tag of specified script as UInt32 in big endian byte order.
 */
//uint32_t SBScriptGetOpenTypeTag(Script script);


}
