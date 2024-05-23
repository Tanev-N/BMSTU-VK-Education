#include <iostream>

bool isLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int getDaysInMonth(int month, int year)
{
    switch (month)
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        return 31;
    case 4:
    case 6:
    case 9:
    case 11:
        return 30;
    case 2:
        return isLeapYear(year) ? 29 : 28;
    default:
        return 0;
    }
}

void subtractOneDay(int &day, int &month, int &year)
{
    day--;
    if (day < 1)
    {
        month--;
        if (month < 1)
        {
            month = 12;
            year--;
        }
        day = getDaysInMonth(month, year);
    }
}

template <typename T, typename C = std::less<T>>
void merge(T *arr, int left, int middle, int right, C cmp = C())
{
    T *arr_temp = new T[right - left + 1];
    int right_temp = middle + 1;
    int left_temp = left;
    int i = 0;
    while (right_temp <= right || left_temp <= middle)
    {
        if (right_temp == right + 1)
        {
            arr_temp[i] = arr[left_temp];
            left_temp++;
        }
        else if (left_temp == middle + 1)
        {
            arr_temp[i] = arr[right_temp];
            right_temp++;
        }
        else
        {
            if (cmp(arr[left_temp], arr[right_temp]))
            {
                arr_temp[i] = arr[left_temp];
                left_temp++;
            }
            else
            {
                arr_temp[i] = arr[right_temp];
                right_temp++;
            }
        }
        i++;
    }
    for (int i = 0; i < right - left + 1; i++)
    {
        arr[left + i] = arr_temp[i];
    }
    delete[] arr_temp;
}

template <typename T, typename C = std::less<T>>
void merge_sort(T *arr, int left, int right, C cmp = C())
{

    if (left >= right)
    {
        return;
    }

    int middle = left + (right - left) / 2;

    merge_sort(arr, left, middle, cmp);
    merge_sort(arr, middle + 1, right, cmp);

    merge(arr, left, middle, right, cmp);
}

struct Human
{
    int birth_year;
    int birth_month;
    int birth_day;

    int death_year;
    int death_month;
    int death_day;
    int live()
    {
        if (birth_month == death_month)
        {
            if (birth_day > death_day)
            {
                return death_year - birth_year - 1;
            }
            else
            {
                return death_year - birth_year;
            }
        }
        else
        {
            if (birth_month > death_month)
            {
                return death_year - birth_year - 1;
            }
            else
            {
                return death_year - birth_year;
            }
        }
    }
};

struct Date_Point
{
    int day;
    int month;
    int year;
    int status;

    Date_Point(int day_ = 0, int month_ = 0, int year_ = 0, int status_ = 0) : day(day_), month(month_), year(year_), status(status_) {}
};

std::istream &operator>>(std::istream &in, Human &a)
{
    in >> a.birth_day >> a.birth_month >> a.birth_year;
    in >> a.death_day >> a.death_month >> a.death_year;
    return in;
}

std::ostream &operator<<(std::ostream &out, Date_Point &a)
{
    out << a.day << " " << a.month << " " << a.year << std::endl;
    return out;
}

std::ostream &operator<<(std::ostream &out, Human &a)
{
    out << a.birth_day << " " << a.birth_month << " " << a.birth_year << " ";
    out << a.death_day << " " << a.death_month << " " << a.death_year << std::endl;
    return out;
}

class Date_Point_Comparator
{

public:
    Date_Point_Comparator(){};
    bool operator()(const Date_Point &a, const Date_Point &b)
    {
        if (a.year == b.year)
        {
            if (a.month == b.month)
            {
                return a.day <= b.day;
            }
            else
            {
                return a.month <= b.month;
            }
        }
        else
        {
            return a.year <= b.year;
        }
    }
};

int main()
{
    int N = 5;
    std::cin >> N;
    Date_Point *arr = new Date_Point[2 * N];
    int count_point = 0;
    for (int i = 0; i < N; i++)
    {
        Human human_temp;
        std::cin >> human_temp;
        if (human_temp.live() >= 18)
        {
            Date_Point date_b(human_temp.birth_day, human_temp.birth_month, human_temp.birth_year + 18, 1);
            arr[count_point] = date_b;
            count_point++;
            if (human_temp.live() >= 80)
            {
                human_temp.birth_year += 80;
                subtractOneDay(human_temp.birth_day, human_temp.birth_month, human_temp.birth_year);
                Date_Point date_d(human_temp.birth_day, human_temp.birth_month, human_temp.birth_year, -1);
                arr[count_point] = date_d;
                count_point++;
            }
            else
            {
                subtractOneDay(human_temp.death_day, human_temp.death_month, human_temp.death_year);
                Date_Point date_d(human_temp.death_day, human_temp.death_month, human_temp.death_year, -1);
                arr[count_point] = date_d;
                count_point++;
            }
        }
    }

    Date_Point_Comparator cmp;

    merge_sort(arr, 0, count_point - 1, cmp);
    // for (int i = 0; i < count_point; i++)
    // {
    //     std::cout << arr[i];
    // }
    int count_sovremenikov = 0;
    int max_count_sovremenikov = 0;

    for (int i = 0; i < count_point; i++)
    {
        count_sovremenikov += arr[i].status;
        // std::cout << count_sovremenikov << std::endl;
        max_count_sovremenikov = std::max(max_count_sovremenikov, count_sovremenikov);
    }

    std::cout << max_count_sovremenikov << std::endl;
    return 0;
}