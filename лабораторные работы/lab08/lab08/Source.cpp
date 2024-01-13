#include <iostream>
using namespace std; 

bool CheckAddress(char* ip_)
{
    int points = 0, 
        numbers = 0; 

    char* buff;

    buff = new char[3]; 

    for (int i = 0; ip_[i] != '\0'; i++)
    { 
        if (ip_[i] <= '9' && ip_[i] >= '0') 
        {
            if (numbers > 3) return false; 
            buff[numbers++] = ip_[i]; 
        }
        else
            if (ip_[i] == '.') 
            {
                if (atoi(buff) > 255)
                    return false;

                if (numbers == 0)
                    return false;

                numbers = 0;
                points++;
                delete[]buff; 
                buff = new char[3];
            }
            else return false;
    }

    if (points != 3)
        return false;

    if (numbers == 0 || numbers > 3)
        return false;

    return true;
}

unsigned long CharToLong(char* ip_)
{
    unsigned long out = 0; 
    char* buff = new char[3];

    for (int i = 0, j = 0, k = 0; ip_[i] != '\0'; i++, j++)
    {
        if (ip_[i] != '.') 
            buff[j] = ip_[i]; 

        if (ip_[i] == '.' || ip_[i + 1] == '\0')
        {
            out <<= 8;

            if (atoi(buff) > 255)
                return NULL;

            out += (unsigned long)atoi(buff); 
            k++;
            j = -1;
            delete[]buff; 
            buff = new char[3]; 
        }
    }
    return out;
}

bool CheckMask(unsigned long mask)
{
    if (mask == 0)
        return false; 

    unsigned long maskTest = 1 << 31; 

    for (int i = 0; i < 32; i++) 
    {
        if (!(mask & maskTest)) 
        {
            for (int j = i + 1; j < 32; j++)
            {
                mask <<= 1;
                if (mask & maskTest)
                    return false; 
            }
        }
        mask <<= 1; 
    }
    return true; 
}

void WriteLongToChar(unsigned long ip)
{
    unsigned long oktet[4]; 
    oktet[0] = ip >> 24;      
    oktet[1] = (ip << 8) >> 24; 
    oktet[2] = (ip << 16) >> 24; 
    oktet[3] = (ip << 24) >> 24; 
    cout << oktet[0] << "." << oktet[1] << "." << oktet[2] << "." << oktet[3] << "\n";
}


int main()
{
    setlocale(LC_ALL, "Russian"); 
    unsigned long ip, mask, host, subnet, broadcast; 
    char* ip_, * mask_;
    bool flag = true; 
    ip_ = new char[16];
    mask_ = new char[16]; 

    do
    {
        if (!flag) cout << "\nНеверно введён адрес!\n" << endl; 
        cout << "введите IP - адрес: \n> ";
        cin >> ip_; 
    } while (!(flag = CheckAddress(ip_))); 

    ip = CharToLong(ip_); 
    flag = true;

    do
    {
        if (!flag)
            cout << "\nНеправильная маска!\n" << endl; 
        flag = true;
        do
        {
            if (!flag)
                cout << "\nНеверно введена маска!\n" << endl; 
            cout << "введите маску подсети: \n> ";
            cin >> mask_; 
        } while (!(flag = CheckAddress(mask_))); 
        mask = CharToLong(mask_); 
    } while (!(flag = CheckMask(mask))); 

    subnet = ip & mask;
    host = ip & ~mask; 
    broadcast = ip & mask | ~mask; 

    cout << "ID подсети:\t\t";
    WriteLongToChar(subnet); 

    cout << "ID хоста:\t\t";
    WriteLongToChar(host); 

    cout << "Broadcast адрес:\t";
    WriteLongToChar(broadcast);

    return 0;
}