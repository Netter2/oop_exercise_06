//Калугин Кирилл Алексеевич М8О-207Б-19
//Создать шаблон динамической коллекцию, согласно варианту задания:
//1.   	Коллекция должна быть реализована с помощью умных указателей (std::shared_ptr, std::weak_ptr). Опционально использование std::unique_ptr.
//2.   	В качестве параметра шаблона коллекция должна принимать тип данных.
//3.   	Коллекция должна содержать метод доступа:
//o	Стек – pop, push, top;
//o	Очередь – pop, push, top;
//o	Список, Динамический массив – доступ к элементу по оператору [].
//4.   	Реализовать аллокатор, который выделяет фиксированный размер памяти (количество блоков памяти – является параметром шаблона аллокатора). Внутри аллокатор должен хранить указатель на используемый блок памяти и динамическую коллекцию указателей на свободные блоки. Динамическая коллекция должна соответствовать варианту задания (Динамический массив, Список, Стек, Очередь).
//5.   	Коллекция должна использовать аллокатор для выделения и освобождения памяти для своих элементов.
//6.   	Аллокатор должен быть совместим с контейнерами std::map и std::list (опционально – vector).
//7.   	Реализовать программу, которая:
//      oпозволяет вводить с клавиатуры фигуры (с типом int в качестве параметра шаблона фигуры) и добавлять в коллекцию использующую аллокатор;
//      oпозволяет удалять элемент из коллекции по номеру элемента;
//      oвыводит на экран введенные фигуры c помощью std::for_each.

//Вариант - пятиугольник; стэк; линейный список;


#include <iostream>
#include <memory>
#include <windows.h>
#include <algorithm>
#include <math.h>
#include <list>
#include <string>

const int inn = 100;

using namespace std;
const double PI = 3.141592653589793238462643383279502884197169399375105820974944;

template <class T, int number>
class Allocator {//класс аллокатора
    public:
        size_t block_size = sizeof (T);
		size_t total_size;
		int free = -1;
		list <T*> pointers;
        T vault [number];
		
        using value_type = T;
        using pointer = T * ;
        using const_pointer = const T*;
        using size_type = size_t;

		void init () {//функция инициализации аллокатора
            total_size = block_size * number;
			for (int i = 0; i < number; i++) {
				pointers.push_back (&vault [i]);
			}
            free = 0;
		}

		T* allocate (int outn) {//функция выделения памяти в аллокаторе
            T* p;
            if (free == -1) {
                init ();
            }

            if ((free + 1) < number) {
                p = pointers.front ();
                for (int i = 0; i < outn; i++) {
                    pointers.pop_front ();
                    free ++;
                }
			}
			else {
			    throw "ERROR\nNo empty space in allocator!";  
            }
			return p;
		}

        template <class U>
        struct rebind {
            using other = Allocator<U, number>;
        };
};

template <typename T>
class Five {//класс пятиугольника
	public:
		using type = T;
		pair<T, T> O;
		T R;
	Five () {};//конструктор класса
	Five (T x, T y, T r): O(x, y), R(r) {};//конструктор класса
};

template <class T>//шаблон функции поиска вершин пятиугольника
void points (T E) {
    Five <double> x = E.vault;
    cout << "----------\n"<< "A = (" << (x.O).first << "," << (x.O).second + x.R << ")\n" << "B = (" 
         << (x.O).first + x.R * cos (28.0 / 180.0 * PI) << "," << (x.O).second + x.R * sin (28.0  / 180.0 * PI) 
         << ")\n" << "C = (" << (x.O).first + x.R * cos (- 44.0  / 180.0 * PI) << "," 
         << (x.O).second + x.R * sin (- 44.0  / 180.0 * PI) << ")\n" << "D = (" 
         << (x.O).first + x.R * cos (- 116.0  / 180.0 * PI) << "," << (x.O).second + x.R * sin (- 116.0  / 180.0 * PI) 
         << ")\n" << "E = (" << (x.O).first + x.R * cos (- 188.0  / 180.0 * PI) 
         << "," << (x.O).second + x.R * sin (- 188.0  / 180.0 * PI) << ")\n" << "----------\n";
}

template <class T>
class Elem {//класс элемента стэка
    public:
        T vault;
        shared_ptr <Elem <T>> prev;   
};

template <class T>
class Iter {//класс итератора
    public:
        weak_ptr <T> iter;

        Iter <T>* operator++ () {//перегруженный оператор ++
            shared_ptr <T> buffer = iter.lock();
            if (buffer) {
                iter = buffer->prev;
            } else {
                throw "Iterator out of range";
            }
            return (this);            
        }

        T operator* () {//перегруженный оператор разыменования
            shared_ptr <T> buffer = iter.lock();
            return (*(buffer.get ()));
        }

        bool operator!= (Iter <T> second) {//перегруженный оператор неравенства
            shared_ptr <T> b1 = iter.lock();
            shared_ptr <T> b2 = second.iter.lock();

            bool ret;

            if (b1.get() != b2.get()) {
                ret = 1;
            }
            else {
                ret = 0;
            }
            return (ret);
        }

        Iter () {};
        Iter (shared_ptr <T> elem) {
            iter = elem;
        };
};

template <class T, class Allocator>
class Stack {//класс стэка
    public:
        shared_ptr <T> first;
        Allocator alloc;
        struct deleter{
            void operator() (void* ptr) {}
        };
        deleter del;

        void push () {//функция вставки на первое место стэка
            double x;
            double y;
            double r;
            T* b;

            cout << "Enter center coordinates and radius: ";
            cin >> x >> y >> r;
            b = alloc.allocate (1);
            shared_ptr <T> curr (new (b) T, del);
            curr->vault.O.first = x;
            curr->vault.O.second = y;
            curr->vault.R = r;
            if (!first) {
                first = curr;
            } else {
                curr->prev = first;
                first = curr;
            }
        }

        void pop () {//функция удаления первого элемента стэка
            if (first) {
                first = first->prev;
                
            } else {
                throw "ERROR\nStack is empty!";
            }
        }

        void top () {//функция просмотра первого элемента
            if (!first) {
                cout << "Stack is empty!\n";
            } else {
                points (*(first.get()));
            }
        }

        Iter <T> begin () {//функция, возвращающая итератор на первый элемент
            if (first) {
                return (Iter <T> (first));
            } else {
                throw "Stack is empty!";
            }
            
        }

        Iter <T> end () {//функция, возвращающая итератор на последний элемент
            shared_ptr <T> buffer = first;
            while (buffer->prev) {
                buffer = buffer->prev;
            }
            return (Iter <T> (buffer->prev)); 
        }

        void delBynumber (int numb) {//функция удаления по номеру
            Iter <T> cur;
            Iter <T> last;
            cur = begin ();
            last = end ();
            if (numb = 1) {
                pop ();
            } else {
                for (int i = 0; i < numb - 2; i++) {
                    if (cur != last) {
                        ++ cur;
                    } else {
                        throw "ERROR\nNo such element in stack!";
                    }
                }
                cur.iter.lock()->prev = cur.iter.lock()->prev->prev;  
            }
        }
};


int main () {
	Stack <Elem <Five <double>>, Allocator <Elem <Five <double>>, inn>> S;
	int option = 99;
	cout << "1.Execute command \"push\"\n";
    cout << "2.Execute command \"pop\"\n";
    cout << "3.Execute command \"top\"\n";
    cout << "4.Show stack\n";
    cout << "5.Delete element by number\n";
    cout << "0.Exit\n";
	while (option != 0) {
		cout << "Choose option: ";
        cin >> option;
        switch (option) {//меню
            case 1: {
                try {
                    S.push ();
                }
                catch (const char* exc) {
                    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 4));
                    cerr << exc << endl;
                    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 7));
                }
                break;              
            }

            case 2: {
                try {
                    S.pop ();
                }
                catch (const char* exc) {
                    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 4));
                    cerr << exc << endl;
                    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 7));
                }
                break;
            }

            case 3: {
                try {
                    S.top ();
                }
                catch (const char* exc) {
                    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 4));
                    cerr << exc << endl;
                    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 7));
                }
                break;
            }

            case 4: {
                if (S.first) {
                    try {
                        if (S.begin () != S.end ()) {
                            if (S.first) {
                                for_each (S.begin (), S.end (), points <Elem <Five <double>>>);
                            }
                        } else {
                            S.top ();
                        }
                    }

                    catch (const char* exc) {
                        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                        SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 4));
                        cerr << exc << endl;
                        SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 7));
                    }
                } else {
                    cout << "Stack is empty!\n";
                }
                break;
            }

            case 5: {
                int x;
                try {
                    cout << "Enter number: ";
                    cin >> x;
                    S.delBynumber (x);
                }
                catch (const char* exc) {
                    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 4));
                    cerr << exc << endl;
                    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 7));
                }
                break;
            }

		case 0: {
                break;
            }

        default: {
            cout << "Wrong command!\n";
            break;
        }
	}
	}

    return(0);
}