  /****************
  *  This project is under development using DAVID: https://github.com/signetlabdei/lorawan/ original class A as a principal source  *
  *  This project can not be used for industrial purpose , it's not updated regularly and some function can be modified or cease to function  *
  *  This project is developed in the purpose of Ph.D. research.
  *  Note that the author of this project: elbsir houssem eddin gives the authorisation for any researcher to use this model with no prior authorization
  ****************/
 
#include "ns3/aes.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("LoRaWANAES");

//NS_OBJECT_ENSURE_REGISTERED (LoRaWANAES);

static BYTE Rcon[] =
{
  0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36
};

static BYTE SBox[] =
{
  0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
  0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
  0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
  0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
  0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
  0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
  0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
  0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
  0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
  0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
  0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
  0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
  0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
  0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
  0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
  0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

static BYTE InvSBox[] =
{
  0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
  0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
  0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
  0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
  0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
  0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
  0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
  0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
  0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
  0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
  0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
  0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
  0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
  0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
  0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
  0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};


AES::AES()
{
  for (int i = 0; i < NB; i++)
    state[i].w = 0x00000000;
  for (int i = 0; i < NB * (NR + 1); i++)
    keySchedule[i].w = 0x00000000;
}

AES::~AES()
{
  for (int i = 0; i < NB; i++)
    state[i].w = 0x00000000;
  for (int i = 0; i < NB * (NR + 1); i++)
    keySchedule[i].w = 0x00000000;
}

void AES::Encrypt(BYTE *input, int size)
{
  int outSize = (size % 16 == 0? size: size + 16 - size % 16);
  BYTE *ptrPos = input;
  //padding
  if (outSize != size)
    {
      ptrPos += size;
      *ptrPos++ = 0x01;
      for (int i = 1; i < outSize - size; i++)
        *ptrPos++ = 0x00;
    }
  ptrPos = input;
  for (int i = 0; i < outSize; i+=16)
    {
      Cipher ((Word *)ptrPos);
      ptrPos+=16;
    }
}

void AES::Decrypt(BYTE *output, int size)
{
  BYTE *ptrPos = output;
  for (int i = 0; i < size; i+=16)
    {
      InvCipher ((Word *)ptrPos);
      ptrPos+=16;
    }
}

void AES::SetKey(BYTE *key, int size)
{
  if (size < KEY_SIZE)
    {
      BYTE *pos = key, *start = key;
      pos+=(size + 1);
      for (int i = size; i < KEY_SIZE; i++, pos++, start++)
        *pos = *start;
    }
  ExpandKey ((Word *)key);
}

void AES::InputToState(Word *input)
{
  Word *pos = input;
  for (int i = 0; i < NB; i++, pos++)
    state[i].w = pos->w;
  pos = 0;
}

void AES::StateToOutput(Word *output)
{
  Word *pos = output;
  for (int i = 0; i < NB; i++, pos++)
    {
      pos->w = state[i].w;
      state[i].w = 0x00000000;
    }
  pos = 0;
}

void AES::Cipher(Word *data)
{
  InputToState (data);

  AddRoundKey (0);

  for (int i = 1; i < NR; i++)
    {
      SubBytes (state[0], false);
      SubBytes (state[1], false);
      SubBytes (state[2], false);
      SubBytes (state[3], false);
      ShiftRows ();
      MixColumns ();
      AddRoundKey (i);
    }

  SubBytes (state[0], false);
  SubBytes (state[1], false);
  SubBytes (state[2], false);
  SubBytes (state[3], false);
  ShiftRows ();
  AddRoundKey (NR);

  StateToOutput (data);
}

void AES::ExpandKey(Word *key)
{
  Word *pos = key, temp;
  for (int i = 0; i < NK; i++)
      keySchedule[i].w = pos++->w;
  for (int i = NK; i < NB * (NR + 1); i++)
    {
      temp.w = keySchedule[i - 1].w;
      if (i % NK == 0)
        {
          RotWord (temp);
          //RotWord (temp, false, 1);
          SubBytes (temp, false);
          temp.b[0] ^= Rcon[i / NK];
        }
      keySchedule[i].w = keySchedule[i - NK].w ^ temp.w;
    }
}

void AES::SubBytes(Word &input, bool isInverse)
{
  if (!isInverse)
    for (int j = 0; j < 4; j++)
      input.b[j] = SBox[input.b[j]];
  else
    for (int j = 0; j < 4; j++)
      input.b[j] = InvSBox[input.b[j]];
}

void AES::ShiftRows()
{
  BYTE temp = 0x00;
  //first row
  temp = state[0].b[1];
  state[0].b[1] = state[1].b[1];
  state[1].b[1] = state[2].b[1];
  state[2].b[1] = state[3].b[1];
  state[3].b[1] = temp;
  //second row
  temp = state[0].b[2];
  state[0].b[2] = state[1].b[2];
  state[1].b[2] = state[2].b[2];
  state[2].b[2] = state[3].b[2];
  state[3].b[2] = temp;
  temp = state[0].b[2];
  state[0].b[2] = state[1].b[2];
  state[1].b[2] = state[2].b[2];
  state[2].b[2] = state[3].b[2];
  state[3].b[2] = temp;
  //third row
  temp = state[3].b[3];
  state[3].b[3] = state[2].b[3];
  state[2].b[3] = state[1].b[3];
  state[1].b[3] = state[0].b[3];
  state[0].b[3] = temp;

  temp = 0x00;
}

void AES::MixColumns()
{
  Word temp;
  for (int i = 0; i < NB; i++)
    {
      temp.w = state[i].w;
      state[i].b[0] = MulBy02 (temp.b[0]) ^ MulBy03 (temp.b[1]) ^ temp.b[2] ^ temp.b[3];
      state[i].b[1] = temp.b[0] ^ MulBy02 (temp.b[1]) ^ MulBy03 (temp.b[2]) ^ temp.b[3];
      state[i].b[2] = temp.b[0] ^ temp.b[1] ^ MulBy02 (temp.b[2]) ^ MulBy03 (temp.b[3]);
      state[i].b[3] = MulBy03 (temp.b[0]) ^ temp.b[1] ^ temp.b[2] ^ MulBy02 (temp.b[3]);
    }
  temp.w = 0x00000000;
}

void AES::AddRoundKey(int round)
{
  for (int i = 0; i < NB; i++)
    state[i].w ^= keySchedule[round*NB + i].w;
}

void AES::RotWord(Word &data, bool isRightRot, int count)
{
  BYTE temp = 0x00;
  if (!isRightRot)
    for (int i = 0; i < count; i++)
      {
        temp = data.b[0];
        data.b[0] = data.b[1];
        data.b[1] = data.b[2];
        data.b[2] = data.b[3];
        data.b[3] = temp;
      }
  else
    for (int i = 0; i < count; i++)
      {
        temp = data.b[3];
        data.b[3] = data.b[2];
        data.b[2] = data.b[1];
        data.b[1] = data.b[0];
        data.b[0] = temp;
      }
  temp = 0x00;
}

void AES::InvCipher(Word *data)
{
  InputToState (data);

  AddRoundKey (NR);

  for (int i = NR - 1; i >= 1; i--)
    {
      InvShiftRows ();
      SubBytes (state[0], true);
      SubBytes (state[1], true);
      SubBytes (state[2], true);
      SubBytes (state[3], true);
      AddRoundKey (i);
      InvMixColumns ();
    }

  InvShiftRows ();
  SubBytes (state[0], true);
  SubBytes (state[1], true);
  SubBytes (state[2], true);
  SubBytes (state[3], true);
  AddRoundKey (0);

  StateToOutput (data);
}

void AES::InvShiftRows()
{
  BYTE temp = 0x00;
  //third row
  temp = state[0].b[3];
  state[0].b[3] = state[1].b[3];
  state[1].b[3] = state[2].b[3];
  state[2].b[3] = state[3].b[3];
  state[3].b[3] = temp;
  //second row
  temp = state[0].b[2];
  state[0].b[2] = state[1].b[2];
  state[1].b[2] = state[2].b[2];
  state[2].b[2] = state[3].b[2];
  state[3].b[2] = temp;
  temp = state[0].b[2];
  state[0].b[2] = state[1].b[2];
  state[1].b[2] = state[2].b[2];
  state[2].b[2] = state[3].b[2];
  state[3].b[2] = temp;
  //first row
  temp = state[3].b[1];
  state[3].b[1] = state[2].b[1];
  state[2].b[1] = state[1].b[1];
  state[1].b[1] = state[0].b[1];
  state[0].b[1] = temp;

  temp = 0x00;
}

void AES::InvMixColumns()
{
  Word temp;
  for (int i = 0; i < NB; i++)
    {
      temp.w = state[i].w;
      state[i].b[0] = MulBy0E (temp.b[0]) ^ MulBy0B (temp.b[1]) ^ MulBy0D (temp.b[2]) ^ MulBy09 (temp.b[3]);
      state[i].b[1] = MulBy09 (temp.b[0]) ^ MulBy0E (temp.b[1]) ^ MulBy0B (temp.b[2]) ^ MulBy0D (temp.b[3]);
      state[i].b[2] = MulBy0D (temp.b[0]) ^ MulBy09 (temp.b[1]) ^ MulBy0E (temp.b[2]) ^ MulBy0B (temp.b[3]);
      state[i].b[3] = MulBy0B (temp.b[0]) ^ MulBy0D (temp.b[1]) ^ MulBy09 (temp.b[2]) ^ MulBy0E (temp.b[3]);
    }
  temp.w = 0x00000000;
}

BYTE AES::MulBy02(BYTE multiplier)
{
  if (multiplier < 0x80)
    return (multiplier << 1);
  else
    return ((multiplier << 1) ^ 0x1b);
}

BYTE AES::MulBy03(BYTE multiplier)
{
  return MulBy02 (multiplier) ^ multiplier;
}

BYTE AES::MulBy09(BYTE multiplier)
{
  return MulBy02 (MulBy02 (MulBy02 (multiplier))) ^ multiplier;
}

BYTE AES::MulBy0B(BYTE multiplier)
{
  return MulBy02 (MulBy02 (MulBy02 (multiplier))) ^ MulBy02 (multiplier) ^ multiplier;
}

BYTE AES::MulBy0D(BYTE multiplier)
{
  return MulBy02 (MulBy02 (MulBy02 (multiplier))) ^ MulBy02 (MulBy02 (multiplier)) ^ multiplier;
}

BYTE AES::MulBy0E(BYTE multiplier)
{
  return MulBy02 (MulBy02 (MulBy02 (multiplier))) ^ MulBy02 (MulBy02 (multiplier)) ^ MulBy02 (multiplier);
}

} // Namespace ns3