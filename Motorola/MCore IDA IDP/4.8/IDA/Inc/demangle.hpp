

#ifndef _DEMANGLE_HPP
#define _DEMANGLE_HPP

// long слово ответов
#define ME_INTERR   -1       // Внутренняя ошибка
#define ME_PARAMERR -2       // Ошибка параметров вызова
#define ME_ILLSTR   -3       // Имя не является корректно замангленным
#define ME_SMALLANS -4       // ответ не влезает в строку
                             // этот ответ бывает только при "старой" форме
                             // вызова. При новой, строка "добивается"
                             // многоточием и ставится знаковый бит
#define ME_FRAME    -5       // могу "частично" разм. (непонятный "хвост")
#define ME_NOCOMP   -6       // не удалось определить компайлер
#define ME_ERRAUTO  -7       // Запрошенный компайлер не совпадает с
                             // автоопределяемым! (для запрошенного не
                             // разманглится.
#define ME_NOHASHMEM -8      // не хватило внутренних индексов - скорее всего
                             // бредовая строка :)
#define ME_NOSTRMEM  -9      // не хватило внутренних буферов (щас :)

#define ME_NOERROR_LIMIT  -10 // значение для проверки на "границу" ошибок
                              // при "усечённой" строке (нехватке выходного
                              // буфера

#define M_PRCMSK     0x0000000F // Если = 0, то данные
#define MT_DEFAULT    0x00000001 // 1 - default (для ваткома/gnu только так)
#define MT_CDECL      0x00000002 // 2 - __cdecl
#define MT_PASCAL     0x00000003 // 3 - __pascal
#define MT_STDCALL    0x00000004 // 4 - __stdcall
#define MT_FASTCALL   0x00000005 // 5 - __fastcall
#define MT_THISCALL   0x00000006 // 6 - __thiscall [ms & bc => pragma only]
#define MT_FORTRAN    0x00000007 // 7 - __fortran
#define MT_SYSCALL    0x00000008 // 8 - __syscall  [without ms]
#define MT_INTERRUPT  0x00000009 // 9 - __interrupt (only with __cdecl!)
#define MT_MSFASTCALL 0x0000000A // A - __msfastcall (bc)
//   зарезервировано на потом
#define MT_LOCALNAME  0x0000000F // f - неизвестно данные или ф-ция. Пока ис-
                                 //     пользуется только для bc - как иденти-
                                 //     фикатор локальных паскалевксих меток
                               // остальное зарезервировано
#define M_SAVEREGS   0x00000010 // Для ф-ций __saveregs
#define M_CLASS     0x000000E0  // 0 - ничего (не member field)
#define MT_PUBLIC    0x00000020 // 1 - public,
#define MT_PRIVATE   0x00000040 // 2 - private,
#define MT_PROTECT   0x00000060 // 3 - protected
#define MT_MEMBER    0x00000080 // 4 - неизвестно к чему относится (bc/wat)
#define MT_VTABLE    0x000000A0 // 5 - таблица вирт. ф-ций класса (bc/gnu)
//   зарезервировано на потом
#define M_PARMSK   0x0000FF00 // маска числа параметров (исключая эллипсис)
                              // 255 - >= 255
#define MT_PARSHF      8    // сдвиг к PARMSK
#define MT_PARMAX     0xFF  // ограничитель числа
#define M_ELLIPSIS 0x00010000 // ф-ция _точно_ имеет '...' в параметрах
#define MT_VOIDARG  0x0001FF00   // если = 0, то ф-ция от (void)
#define M_STATIC   0x00020000 // static
#define M_VIRTUAL  0x00040000 // virtual
#define M_AUTOCRT  0x00080000   // Скорее всего "автопорождённая" ф-ция (дата)
#define M_TYPMASK  0x00700000   // специальные случаи func (0 обычная)
#define MT_OPERAT   0x00100000    // 1 - оператор
#define MT_CONSTR   0x00200000    // 2 - конструктор
#define MT_DESTR    0x00300000    // 3 - деструктор
#define MT_CASTING  0x00400000    // 4 - оператор преобразования типа
                             // прочее зарезервировано на потом
#define M_TRUNCATE 0x00800000   // Имя УСЕЧЕНО манглером (bc/va)
#define M_THUNK    0x01000000 // [thunk]:
#define M_ANONNSP  0x02000000 // ms  => Anonymous Namespace for field
                              // wat => anonymous_enum
                              // bc  => + TMPLNAM = PascalTemplate (for DCC)
                              //  Если отдельно - "автоматический" except_t
                              //  от билдера для "внешних" переменных
                              //  или его же темплейт для таблиц конструк-
                              //  торов/деструкторов глобальных объектов
#define M_TMPLNAM  0x04000000 // ms  => Имя темплейта (?)
                              // wat =>
                              // bc  => Имя темплейта => табл. его описания
#define M_DBGNAME  0x08000000 // ms  =>  CV:
                              // wat => xxxx: (T?xxxx-form)
                              // bc  => old pascal format (capitalized)
#define M_COMPILER 0x70000000 // маска типа манглера (0 неизвестно)
#define MT_MSCOMP   0x10000000  // 1 - мелкософт/симантек
#define MT_BORLAN   0x20000000  // 2 - борланд
#define MT_WATCOM   0x30000000  // 3 - ватком
//#define MT_         0x40000000  // 4 - резерв
//#define MT_         0x50000000  // 5 - резерв
// !!! Следующие обязательно последние и именно в таком порядке!
#define MT_GNU      0x60000000  // 6 - GNU - (поверх VA для автомата)
#define MT_VISAGE   0x70000000  // 7 - Visual Age - ТОЛЬКО _не_ автомат!
                                 // при _короткой_ форме этот ответ означает
                                 // VA _или_ GNU. Причём на автомате будет
                                 // вызываться GNU!
//========================================================================
// Определения для disable_flags
#define MNG_NOPTRTYP     0x00000001 // Давить near/far/huge
// следующую группу двигать нелья и нельзя использовать промежутки - PtrType
#define MNG_PTRMSK       0x6  // маска модели (если нет NOPTRTYP)
#define MNG_DEFNEAR       0x0 // near давится, остальное выводится
#define MNG_DEFFAR        0x2 // far давится, остальное выводится
#define MNG_DEFHUGE       0x4 // huge давится, остальное выводится
#define MNG_DEFNONE       0x6 // выводится всё
//
#define MNG_NODEFINIT    0x00000008 // Давить всё кроме основного имени
//
#define MNG_NOUNDERSCORE 0x00000010 // Давить подчерки для __ccall, __pascal...        +
#define MNG_NOTYPE       0x00000020 // Давить callc&based своё и параметров
#define MNG_NORETTYPE    0x00000040 // Давить return type процедур
#define MNG_NOBASEDT     0x00000080 // Давить типы на кот. "базируемся"
#define MNG_NOCALLC      0x00000100 // Давить __pascal/__ccall/etc
#define MNG_NOPOSTFC     0x00000200 // Давить постфиксный const
#define MNG_NOSCTYP      0x00000400 // Давить public/private/protected
#define MNG_NOTHROW      0x00000800 // Давить описание throw
#define MNG_NOSTVIR      0x00001000 // Давить слова static и virtual
#define MNG_NOECSU       0x00002000 // Давить слова class/struct/union/enum
#define MNG_NOCSVOL      0x00004000 // Давить слова const и volatile всюду
#define MNG_NOCLOSUR     0x00008000 // Давить __closure у борланда
//
#define MNG_SHORT_S      0x00010000 // signed (int) в формате s(int)
#define MNG_SHORT_U      0x00020000 // unsigned (int) в формате u(int)
#define MNG_ZPT_SPACE    0x00040000 // Выв. пробел после запятой в аргум.
//
#define MNG_IGN_ANYWAY   0x00080000 // Игнорировать '_nn' в конце строки
#define MNG_IGN_JMP      0x00100000 // Игнорировать 'j_'  в начале строки
#define MNG_MOVE_JMP     0x00200000 // Переносить 'j_' в размангленное
                                      // Если установлены оба, то перемещение
                                      // только для не усечённого
#define MNG_DROP_IMP     0x00400000 // не выводить __declspec(dllimport)
//
#define MNG_COMPILER_MSK 0x70000000 // маска компилятора (0 - автомат)

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

// Если answer_length == 0, это "короткая" форма - размангливание не
// производится. Осуществляется только короткая проверка того можно ли
// пытаться интерпретировать данное имя как замангленное. Если вероятность
// отлична от 0 :), то в ответе _каким_ деманглером. Иначе 0.
// ПРИМЕЧАНИЕ: Если ответ MT_MSCOMP+1, то это __msfastcall
//             (или борладновсикй класс вида _4 :), и деманглить можно,
//             как ms (__fastcall), так и bc (__msfastcall)

// ВНИМАНИЕ: Если (long)answer_length < 0 то в качестве размера буфера
//           берётся -answer_length НО answer интерпретируется не как
//           адрес буфера, а как адрес адреса буфера (char **).
//           В этом случае при "положительном" коде ответа в этой
//           переменной возвращается адрес конца вывода ('\0')
//           (аналогия - stpcpy)
//           При такой форме НЕЛЬЗЯ указывать answer==NULL
//           а если *(char **)answer == NULL это сичтается ошибкой параметров
//           При такаой форме answer_length должен быть НЕ МЕНЬШЕ 10
//           (иначе ошибка параметров) для "усечённого" ответа

// if answer == NULL then demangler will return the flags only
// in this case answer_length should be enough to hold the demangled name

typedef int mangled_name_type_t;
const mangled_name_type_t MANGLED_CODE = 0;
const mangled_name_type_t MANGLED_DATA = 1;
const mangled_name_type_t MANGLED_UNKNOWN = 2;

idaman mangled_name_type_t ida_export get_mangled_name_type(const char *name);

#endif // H2ASH
#endif // _DEMANGLE_HPP
