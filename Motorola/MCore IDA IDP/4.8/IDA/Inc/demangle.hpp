

#ifndef _DEMANGLE_HPP
#define _DEMANGLE_HPP

// long ᫮�� �⢥⮢
#define ME_INTERR   -1       // ����७��� �訡��
#define ME_PARAMERR -2       // �訡�� ��ࠬ��஢ �맮��
#define ME_ILLSTR   -3       // ��� �� ���� ���४⭮ �����������
#define ME_SMALLANS -4       // �⢥� �� ������� � ��ப�
                             // ��� �⢥� �뢠�� ⮫쪮 �� "��ன" �ଥ
                             // �맮��. �� �����, ��ப� "����������"
                             // ������稥� � �⠢���� ������� ���
#define ME_FRAME    -5       // ���� "���筮" ࠧ�. (�������� "墮��")
#define ME_NOCOMP   -6       // �� 㤠���� ��।����� ���������
#define ME_ERRAUTO  -7       // ����襭�� ��������� �� ᮢ������ �
                             // ��⮮�।��塞�! (��� ����襭���� ��
                             // ࠧ���������.
#define ME_NOHASHMEM -8      // �� 墠⨫� ����७��� �����ᮢ - ᪮॥ �ᥣ�
                             // �।���� ��ப� :)
#define ME_NOSTRMEM  -9      // �� 墠⨫� ����७��� ���஢ (�� :)

#define ME_NOERROR_LIMIT  -10 // ���祭�� ��� �஢�ન �� "�࠭���" �訡��
                              // �� "���񭭮�" ��ப� (��墠⪥ ��室����
                              // ����

#define M_PRCMSK     0x0000000F // �᫨ = 0, � �����
#define MT_DEFAULT    0x00000001 // 1 - default (��� ��⪮��/gnu ⮫쪮 ⠪)
#define MT_CDECL      0x00000002 // 2 - __cdecl
#define MT_PASCAL     0x00000003 // 3 - __pascal
#define MT_STDCALL    0x00000004 // 4 - __stdcall
#define MT_FASTCALL   0x00000005 // 5 - __fastcall
#define MT_THISCALL   0x00000006 // 6 - __thiscall [ms & bc => pragma only]
#define MT_FORTRAN    0x00000007 // 7 - __fortran
#define MT_SYSCALL    0x00000008 // 8 - __syscall  [without ms]
#define MT_INTERRUPT  0x00000009 // 9 - __interrupt (only with __cdecl!)
#define MT_MSFASTCALL 0x0000000A // A - __msfastcall (bc)
//   ��१�ࢨ஢��� �� ��⮬
#define MT_LOCALNAME  0x0000000F // f - �������⭮ ����� ��� �-��. ���� ��-
                                 //     �������� ⮫쪮 ��� bc - ��� �����-
                                 //     䨪��� �������� ��᪠������ ��⮪
                               // ��⠫쭮� ��१�ࢨ஢���
#define M_SAVEREGS   0x00000010 // ��� �-権 __saveregs
#define M_CLASS     0x000000E0  // 0 - ��祣� (�� member field)
#define MT_PUBLIC    0x00000020 // 1 - public,
#define MT_PRIVATE   0x00000040 // 2 - private,
#define MT_PROTECT   0x00000060 // 3 - protected
#define MT_MEMBER    0x00000080 // 4 - �������⭮ � 祬� �⭮���� (bc/wat)
#define MT_VTABLE    0x000000A0 // 5 - ⠡��� ����. �-権 ����� (bc/gnu)
//   ��१�ࢨ஢��� �� ��⮬
#define M_PARMSK   0x0000FF00 // ��᪠ �᫠ ��ࠬ��஢ (�᪫��� �����)
                              // 255 - >= 255
#define MT_PARSHF      8    // ᤢ�� � PARMSK
#define MT_PARMAX     0xFF  // ��࠭��⥫� �᫠
#define M_ELLIPSIS 0x00010000 // �-�� _�筮_ ����� '...' � ��ࠬ����
#define MT_VOIDARG  0x0001FF00   // �᫨ = 0, � �-�� �� (void)
#define M_STATIC   0x00020000 // static
#define M_VIRTUAL  0x00040000 // virtual
#define M_AUTOCRT  0x00080000   // ���॥ �ᥣ� "��⮯�஦�񭭠�" �-�� (���)
#define M_TYPMASK  0x00700000   // ᯥ樠��� ��砨 func (0 ���筠�)
#define MT_OPERAT   0x00100000    // 1 - ������
#define MT_CONSTR   0x00200000    // 2 - ���������
#define MT_DESTR    0x00300000    // 3 - ��������
#define MT_CASTING  0x00400000    // 4 - ������ �८�ࠧ������ ⨯�
                             // ��祥 ��१�ࢨ஢��� �� ��⮬
#define M_TRUNCATE 0x00800000   // ��� ������� ������஬ (bc/va)
#define M_THUNK    0x01000000 // [thunk]:
#define M_ANONNSP  0x02000000 // ms  => Anonymous Namespace for field
                              // wat => anonymous_enum
                              // bc  => + TMPLNAM = PascalTemplate (for DCC)
                              //  �᫨ �⤥�쭮 - "��⮬���᪨�" except_t
                              //  �� ������ ��� "���譨�" ��६�����
                              //  ��� ��� �� ⥬����� ��� ⠡��� �������-
                              //  �஢/�������஢ ��������� ��ꥪ⮢
#define M_TMPLNAM  0x04000000 // ms  => ��� ⥬����� (?)
                              // wat =>
                              // bc  => ��� ⥬����� => ⠡�. ��� ���ᠭ��
#define M_DBGNAME  0x08000000 // ms  =>  CV:
                              // wat => xxxx: (T?xxxx-form)
                              // bc  => old pascal format (capitalized)
#define M_COMPILER 0x70000000 // ��᪠ ⨯� ������� (0 �������⭮)
#define MT_MSCOMP   0x10000000  // 1 - ��������/ᨬ��⥪
#define MT_BORLAN   0x20000000  // 2 - ��ૠ��
#define MT_WATCOM   0x30000000  // 3 - ��⪮�
//#define MT_         0x40000000  // 4 - १��
//#define MT_         0x50000000  // 5 - १��
// !!! ������騥 ��易⥫쭮 ��᫥���� � ������ � ⠪�� ���浪�!
#define MT_GNU      0x60000000  // 6 - GNU - (������ VA ��� ��⮬��)
#define MT_VISAGE   0x70000000  // 7 - Visual Age - ������ _��_ ��⮬��!
                                 // �� _���⪮�_ �ଥ ��� �⢥� ����砥�
                                 // VA _���_ GNU. ���� �� ��⮬�� �㤥�
                                 // ��뢠���� GNU!
//========================================================================
// ��।������ ��� disable_flags
#define MNG_NOPTRTYP     0x00000001 // ������ near/far/huge
// ᫥������ ��㯯� ������� ����� � ����� �ᯮ�짮���� �஬���⪨ - PtrType
#define MNG_PTRMSK       0x6  // ��᪠ ������ (�᫨ ��� NOPTRTYP)
#define MNG_DEFNEAR       0x0 // near �������, ��⠫쭮� �뢮�����
#define MNG_DEFFAR        0x2 // far �������, ��⠫쭮� �뢮�����
#define MNG_DEFHUGE       0x4 // huge �������, ��⠫쭮� �뢮�����
#define MNG_DEFNONE       0x6 // �뢮����� ���
//
#define MNG_NODEFINIT    0x00000008 // ������ ��� �஬� �᭮����� �����
//
#define MNG_NOUNDERSCORE 0x00000010 // ������ ����ન ��� __ccall, __pascal...        +
#define MNG_NOTYPE       0x00000020 // ������ callc&based ᢮� � ��ࠬ��஢
#define MNG_NORETTYPE    0x00000040 // ������ return type ��楤��
#define MNG_NOBASEDT     0x00000080 // ������ ⨯� �� ���. "�����㥬��"
#define MNG_NOCALLC      0x00000100 // ������ __pascal/__ccall/etc
#define MNG_NOPOSTFC     0x00000200 // ������ ����䨪�� const
#define MNG_NOSCTYP      0x00000400 // ������ public/private/protected
#define MNG_NOTHROW      0x00000800 // ������ ���ᠭ�� throw
#define MNG_NOSTVIR      0x00001000 // ������ ᫮�� static � virtual
#define MNG_NOECSU       0x00002000 // ������ ᫮�� class/struct/union/enum
#define MNG_NOCSVOL      0x00004000 // ������ ᫮�� const � volatile ����
#define MNG_NOCLOSUR     0x00008000 // ������ __closure � ��ૠ���
//
#define MNG_SHORT_S      0x00010000 // signed (int) � �ଠ� s(int)
#define MNG_SHORT_U      0x00020000 // unsigned (int) � �ଠ� u(int)
#define MNG_ZPT_SPACE    0x00040000 // ��. �஡�� ��᫥ ����⮩ � ���.
//
#define MNG_IGN_ANYWAY   0x00080000 // �����஢��� '_nn' � ���� ��ப�
#define MNG_IGN_JMP      0x00100000 // �����஢��� 'j_'  � ��砫� ��ப�
#define MNG_MOVE_JMP     0x00200000 // ��७���� 'j_' � ࠧ����������
                                      // �᫨ ��⠭������ ���, � ��६�饭��
                                      // ⮫쪮 ��� �� ���񭭮��
#define MNG_DROP_IMP     0x00400000 // �� �뢮���� __declspec(dllimport)
//
#define MNG_COMPILER_MSK 0x70000000 // ��᪠ ��������� (0 - ��⮬��)

#define MNG_SHORT_FORM  (MNG_NOTYPE|MNG_NORETTYPE|MNG_NOPOSTFC|MNG_NOPTRTYP| \
        MNG_NOSCTYP|MNG_NOTHROW|MNG_NOSTVIR|MNG_NOECSU|MNG_NOCLOSUR| \
             MNG_SHORT_U|MNG_IGN_JMP|MNG_MOVE_JMP|MNG_IGN_ANYWAY| \
             MNG_DROP_IMP)
#define MNG_LONG_FORM (MNG_ZPT_SPACE | MNG_IGN_JMP | MNG_IGN_ANYWAY)

//========================================================================
#ifndef H2ASH

typedef signed long ida_export demangler_t(char *answer, unsigned answer_length,
                                const char *str,
                                ulong disable_mask);

idaman demangler_t demangle;

// �᫨ answer_length == 0, �� "���⪠�" �ଠ - ࠧ����������� ��
// �ந��������. �����⢫���� ⮫쪮 ���⪠� �஢�ઠ ⮣� ����� ��
// ������� ������஢��� ������ ��� ��� ������������. �᫨ ����⭮���
// �⫨筠 �� 0 :), � � �⢥� _�����_ ��������஬. ���� 0.
// ����������: �᫨ �⢥� MT_MSCOMP+1, � �� __msfastcall
//             (��� ��ૠ����ᨪ� ����� ���� _4 :), � ���������� �����,
//             ��� ms (__fastcall), ⠪ � bc (__msfastcall)

// ��������: �᫨ (long)answer_length < 0 � � ����⢥ ࠧ��� ����
//           ������� -answer_length �� answer ����������� �� ���
//           ���� ����, � ��� ���� ���� ���� (char **).
//           � �⮬ ��砥 �� "������⥫쭮�" ���� �⢥� � �⮩
//           ��६����� �����頥��� ���� ���� �뢮�� ('\0')
//           (�������� - stpcpy)
//           �� ⠪�� �ଥ ������ 㪠�뢠�� answer==NULL
//           � �᫨ *(char **)answer == NULL �� ��⠥��� �訡��� ��ࠬ��஢
//           �� ⠪��� �ଥ answer_length ������ ���� �� ������ 10
//           (���� �訡�� ��ࠬ��஢) ��� "���񭭮��" �⢥�

// if answer == NULL then demangler will return the flags only
// in this case answer_length should be enough to hold the demangled name

typedef int mangled_name_type_t;
const mangled_name_type_t MANGLED_CODE = 0;
const mangled_name_type_t MANGLED_DATA = 1;
const mangled_name_type_t MANGLED_UNKNOWN = 2;

idaman mangled_name_type_t ida_export get_mangled_name_type(const char *name);

#endif // H2ASH
#endif // _DEMANGLE_HPP
