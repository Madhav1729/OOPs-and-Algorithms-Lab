#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <string>
#define endl std::endl
const int inf = 2e9 + 1;

class Card
{
    public:
        Card() {}

        bool operator<(Card& b)
        {
            return area_ < b.area_;
        }
        
        virtual void printCard()
        {

        }
        virtual void takeInput()
        {

        }

        friend class CardOrganizer;

    protected:
        int area_;
};

std::istream& operator>>(std::istream& in, Card& b)
{
    b.takeInput();
    return in;
}

std::ostream& operator<<(std::ostream& out, Card& b)
{
    b.printCard();
    return out;
}

class Square : public Card
{
    public:
        Square() : Card() {}

        void takeInput()
        {
            std::cin >> edge_;
            area_ = edge_ * edge_;
        }

        void printCard()
        {
            std::cout << "SQUARE " << edge_ << endl;
        }

    private:
        int edge_;
};

class Rectangle : public Card
{
    public:
        Rectangle() : Card() {}

        void takeInput()
        {
            std::cin >> length_ >> breadth_;
            area_ = length_ * breadth_;
        }

        void printCard()
        {
            std::cout << "RECTANGLE " << length_ << " " << breadth_ << endl;
        }

    private:
        int length_;
        int breadth_;
};

class Triangle : public Card
{
    public:
        Triangle() : Card() {}

        void takeInput()
        {
            std::cin >> base_ >> height_;
            area_ = (base_ * height_) / 2;
        }

        void printCard()
        {
            std::cout << "TRIANGLE " << base_ << " " << height_ << endl;
        }

    private:
        int base_;
        int height_;
};

class Comparator
{
    public:
        bool operator()(std::pair<int,int>& a, int val)
        {
            return a.first < val;
        }
};

class CardOrganizer
{
    public:
        CardOrganizer(int n) : LISArray(n, {inf, -1})
        {
            LISArray[0] = {0, -1};
            cards.reserve(n);
        }
        CardOrganizer() {}

        void addCard(Card* nCard)
        {
            //inserts card into LISArray   
            std::cin >> (*nCard);         
            cards.push_back({nCard->area_, nCard});
            parent.push_back(-1);
            
            int i = cards.size() - 1;
            int index = std::lower_bound(LISArray.begin(), LISArray.end(), cards[i].first, Comparator()) - LISArray.begin();
            
            LISArray[index] = {cards[i].first, i};
            parent[i] = LISArray[index - 1].second;
        }

        size_t getPileCount() const
        {
            // returns index of farthest non empty element of LISArray
            int i = 0, j = LISArray.size();
            while (j > i)
            {
                int mid = (i + j)/2;
                if (LISArray[mid].first == inf) j = mid;
                else i = mid + 1;
            }

            return i - 1;
        }

        std::vector<Card*> getLIS() const
        {
            //returns LIS
            int i = 0, j = LISArray.size();
            while (j > i)
            {
                int mid = (i + j)/2;
                if (LISArray[mid].first == inf) j = mid;
                else i = mid + 1;
            }

            std::vector<Card*> res;
            i = LISArray[i - 1].second;
            while (i != -1)
            {
                res.push_back(cards[i].second);
                i = parent[i];
            }
            std::reverse(res.begin(), res.end());
            
            return res;
        }

    private:
        std::vector<std::pair<int,Card*>> cards;
        std::vector<std::pair<int,int>> LISArray;
        std::vector<int> parent;
};

int main()
{
    int q;
    std::cin >> q;

    CardOrganizer og(q);
    std::vector<Square> squares;
    std::vector<Rectangle> rectangles;
    std::vector<Triangle> triangles;
    squares.reserve(q);
    rectangles.reserve(q);
    triangles.reserve(q);

    int type;
    while(q--)
    {
        std::cin >> type;
        if (type == 1)
        {
            std::string shape;
            std::cin >> shape;

            if (shape == "SQUARE")
            {
                Square nCard;
                squares.push_back(nCard);
                og.addCard(&squares.back());
            }
            else if (shape == "RECTANGLE")
            {
                Rectangle nCard;
                rectangles.push_back(nCard);
                og.addCard(&rectangles.back());
            }
            else
            {
                Triangle nCard;
                triangles.push_back(nCard);
                og.addCard(&triangles.back());
            }
        }
        else if (type == 2)
        {
            std::cout << og.getPileCount() << endl;
        }
        else
        {
            std::vector<Card*> LIS = og.getLIS();

            std::cout << LIS.size() << endl;
            for (auto &x : LIS)
            {
                x->printCard();
            }
        }
    }
}
