#include "inputmap.h"

InputMap *inputMap = nullptr;

InputMap::InputMap() {
    initKeyMap();
    initModMap();
}

InputMap *InputMap::getInstance() {
    if(!inputMap) {
       inputMap = new InputMap();
    }
    return inputMap;
}

const QMap<int, QString> &InputMap::keys() {
    return keyMap;
}

const QMap<QString, int> &InputMap::keysReverse() {
    return keyMapReverse;
}

const QMap<QString, Qt::KeyboardModifier> &InputMap::modifiers() {
    return modMap;
}

void InputMap::initKeyMap() {
    // layout-independent key codes
    // TODO: sell a kidney and add key codes from OS X
    keyMap.clear();
#ifdef _WIN32
    keyMap.insert(16, "Q");
    keyMap.insert(17, "W");
    keyMap.insert(18, "E");
    keyMap.insert(19, "R");
    keyMap.insert(20, "T");
    keyMap.insert(21, "Y");
    keyMap.insert(22, "U");
    keyMap.insert(23, "I");
    keyMap.insert(24, "O");
    keyMap.insert(25, "P");
    keyMap.insert(30, "A");
    keyMap.insert(31, "S");
    keyMap.insert(32, "D");
    keyMap.insert(33, "F");
    keyMap.insert(34, "G");
    keyMap.insert(35, "H");
    keyMap.insert(36, "J");
    keyMap.insert(37, "K");
    keyMap.insert(38, "L");
    keyMap.insert(44, "Z");
    keyMap.insert(45, "X");
    keyMap.insert(46, "C");
    keyMap.insert(47, "V");
    keyMap.insert(48, "B");
    keyMap.insert(49, "N");
    keyMap.insert(50, "M");
    keyMap.insert(331, "Left");
    keyMap.insert(333, "Right");
    keyMap.insert(328, "Up");
    keyMap.insert(336, "Down");
    keyMap.insert(57, "Space");
    keyMap.insert(28, "Return");
    keyMap.insert(15, "Tab");
    keyMap.insert(59, "F1");
    keyMap.insert(60, "F2");
    keyMap.insert(61, "F3");
    keyMap.insert(62, "F4");
    keyMap.insert(63, "F5");
    keyMap.insert(64, "F6");
    keyMap.insert(65, "F7");
    keyMap.insert(66, "F8");
    keyMap.insert(67, "F9");
    keyMap.insert(68, "F10");
    keyMap.insert(87, "F11");
    keyMap.insert(88, "F12");
    keyMap.insert(339, "delete");
    keyMap.insert(1, "escape");
    keyMap.insert(329, "pageUp");
    keyMap.insert(337, "pageDown");
    keyMap.insert(41, "~");
    keyMap.insert(2, "1");
    keyMap.insert(3, "2");
    keyMap.insert(4, "3");
    keyMap.insert(5, "4");
    keyMap.insert(6, "5");
    keyMap.insert(7, "6");
    keyMap.insert(8, "7");
    keyMap.insert(9, "8");
    keyMap.insert(10, "9");
    keyMap.insert(11, "0");
    keyMap.insert(12,"-");
    keyMap.insert(13,"=");
    keyMap.insert(327, "home");
    keyMap.insert(335, "end");
    keyMap.insert(14, "backspace");
#elif defined __linux__
    keyMap.insert(24, "Q");
    keyMap.insert(25, "W");
    keyMap.insert(26, "E");
    keyMap.insert(27, "R");
    keyMap.insert(28, "T");
    keyMap.insert(29, "Y");
    keyMap.insert(30, "U");
    keyMap.insert(31, "I");
    keyMap.insert(32, "O");
    keyMap.insert(33, "P");
    keyMap.insert(34, "[");
    keyMap.insert(35, "]");
    keyMap.insert(38, "A");
    keyMap.insert(39, "S");
    keyMap.insert(40, "D");
    keyMap.insert(41, "F");
    keyMap.insert(42, "G");
    keyMap.insert(43, "H");
    keyMap.insert(44, "J");
    keyMap.insert(45, "K");
    keyMap.insert(46, "L");
    keyMap.insert(47, ";");
    keyMap.insert(48, "'");
    keyMap.insert(52, "Z");
    keyMap.insert(53, "X");
    keyMap.insert(54, "C");
    keyMap.insert(55, "V");
    keyMap.insert(56, "B");
    keyMap.insert(57, "N");
    keyMap.insert(58, "M");
    keyMap.insert(59, ",");
    keyMap.insert(60, ".");
    keyMap.insert(61, "/");
    keyMap.insert(113, "Left");
    keyMap.insert(114, "Right");
    keyMap.insert(111, "Up");
    keyMap.insert(116, "Down");
    keyMap.insert(65, "Space");
    keyMap.insert(36, "Return");
    keyMap.insert(23, "Tab");
    keyMap.insert(67, "F1");
    keyMap.insert(68, "F2");
    keyMap.insert(69, "F3");
    keyMap.insert(70, "F4");
    keyMap.insert(71, "F5");
    keyMap.insert(72, "F6");
    keyMap.insert(73, "F7");
    keyMap.insert(74, "F8");
    keyMap.insert(75, "F9");
    keyMap.insert(76, "F10");
    keyMap.insert(95, "F11");
    keyMap.insert(96, "F12");
    keyMap.insert(119, "delete");
    keyMap.insert(9, "escape");
    keyMap.insert(112, "pageUp");
    keyMap.insert(117, "pageDown");
    keyMap.insert(49, "~");
    keyMap.insert(10, "1");
    keyMap.insert(11, "2");
    keyMap.insert(12, "3");
    keyMap.insert(13, "4");
    keyMap.insert(14, "5");
    keyMap.insert(15, "6");
    keyMap.insert(16, "7");
    keyMap.insert(17, "8");
    keyMap.insert(18, "9");
    keyMap.insert(19, "0");
    keyMap.insert(20, "-");
    keyMap.insert(21, "=");
    keyMap.insert(22, "backspace");
    keyMap.insert(110, "home");
    keyMap.insert(115, "end");
//////////////////////////////////////////
    keyMapReverse.insert("Q", 24);
    keyMapReverse.insert("W", 25);
    keyMapReverse.insert("E", 26);
    keyMapReverse.insert("R", 27);
    keyMapReverse.insert("T", 28);
    keyMapReverse.insert("Y", 29);
    keyMapReverse.insert("U", 30);
    keyMapReverse.insert("I", 31);
    keyMapReverse.insert("O", 32);
    keyMapReverse.insert("P", 33);
    keyMapReverse.insert("[", 34);
    keyMapReverse.insert("]", 35);
    keyMapReverse.insert("A", 38);
    keyMapReverse.insert("S", 39);
    keyMapReverse.insert("D", 40);
    keyMapReverse.insert("F", 41);
    keyMapReverse.insert("G", 42);
    keyMapReverse.insert("H", 43);
    keyMapReverse.insert("J", 44);
    keyMapReverse.insert("K", 45);
    keyMapReverse.insert("L", 46);
    keyMapReverse.insert(";", 47);
    keyMapReverse.insert("'", 48);
    keyMapReverse.insert("Z", 52);
    keyMapReverse.insert("X", 53);
    keyMapReverse.insert("C", 54);
    keyMapReverse.insert("V", 55);
    keyMapReverse.insert("B", 56);
    keyMapReverse.insert("N", 57);
    keyMapReverse.insert("M", 58);
    keyMapReverse.insert(",", 59);
    keyMapReverse.insert(".", 60);
    keyMapReverse.insert("/", 61);
    keyMapReverse.insert("Left", 113);
    keyMapReverse.insert("Right", 114);
    keyMapReverse.insert("Up", 111);
    keyMapReverse.insert("Down", 116);
    keyMapReverse.insert("Space", 65);
    keyMapReverse.insert("Return", 36);
    keyMapReverse.insert("Tab", 23);
    keyMapReverse.insert("F1", 67);
    keyMapReverse.insert("F2", 68);
    keyMapReverse.insert("F3", 69);
    keyMapReverse.insert("F4", 70);
    keyMapReverse.insert("F5", 71);
    keyMapReverse.insert("F6", 72);
    keyMapReverse.insert("F7", 73);
    keyMapReverse.insert("F8", 74);
    keyMapReverse.insert("F9", 75);
    keyMapReverse.insert("F10", 76);
    keyMapReverse.insert("F11", 95);
    keyMapReverse.insert("F12", 96);
    keyMapReverse.insert("delete", 119);
    keyMapReverse.insert("escape", 9);
    keyMapReverse.insert("pageUp", 112);
    keyMapReverse.insert("pageDown", 117);
    keyMapReverse.insert("~", 49);
    keyMapReverse.insert("1", 10);
    keyMapReverse.insert("2", 11);
    keyMapReverse.insert("3", 12);
    keyMapReverse.insert("4", 13);
    keyMapReverse.insert("5", 14);
    keyMapReverse.insert("6", 15);
    keyMapReverse.insert("7", 16);
    keyMapReverse.insert("8", 17);
    keyMapReverse.insert("9", 18);
    keyMapReverse.insert("0", 19);
    keyMapReverse.insert("-", 20);
    keyMapReverse.insert("=", 21);
    keyMapReverse.insert("backspace", 22);
    keyMapReverse.insert("home", 110);
    keyMapReverse.insert("end", 115);
#endif
}

void InputMap::initModMap() {
    modMap.clear();
    modMap.insert("Shift+", Qt::ShiftModifier);
    modMap.insert("Ctrl+", Qt::ControlModifier);
    modMap.insert("Alt+", Qt::AltModifier);
}
