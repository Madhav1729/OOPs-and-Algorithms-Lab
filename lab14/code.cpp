#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>

class Comparator;
class EventScheduler;
class lowerBoundComparator;

class Event
{
    public:
        Event(int startTime, int endTime) : startTime_(startTime), endTime_(endTime) {}
        Event() {}

        virtual double calculateProfit() = 0;

        virtual void takeInput() = 0;

        friend class EventScheduler;
        friend class Comparator;
        friend class lowerBoundComparator;

    protected:
        int startTime_;
        int endTime_;
};

std::istream& operator>>(std::istream& in, Event& b)
{
    b.takeInput();
    return in;
}

class Concert : public Event
{
    public:
        Concert(int sTime, int eTime, int ticketPrice, int ticketsSold, int artistFee,int logisticsCost) : Event(sTime, eTime)
        {
            ticketPrice_ = ticketPrice;
            ticketsSold_ = ticketsSold;
            artistFee_ = artistFee;
            logisticsCost_ = logisticsCost;
        }
        Concert() : Event() {}

        double calculateProfit()
        {
            double netProfit = ticketPrice_ * ticketsSold_;
            netProfit = ((double)1 - 0.18) * netProfit;
            netProfit -= logisticsCost_ + artistFee_;
            return netProfit = (netProfit > 2 * (logisticsCost_ + artistFee_) ? 0.7 * netProfit : netProfit);
        }

        void takeInput()
        {
            std::cin >> startTime_ >> endTime_ >> ticketPrice_ >> ticketsSold_ >> artistFee_ >> logisticsCost_;
        }

    private:
        int ticketPrice_;
        int ticketsSold_;
        int artistFee_;
        int logisticsCost_;
};

class TheatreShow : public Event
{
    public:
        TheatreShow(int sTime, int eTime, int baseTicketPrice, int totalSeats, int venueCost) : Event(sTime, eTime)
        {
            baseTicketPrice_ = baseTicketPrice;
            totalSeats_ = totalSeats;
            venueCost_ = venueCost;
        }
        TheatreShow() : Event() {}

        double calculateProfit()
        {
            double netProfit = (totalSeats_ / 4) * 2 * baseTicketPrice_ + (3 * totalSeats_ / 4) * baseTicketPrice_;
            netProfit = netProfit - 0.18 * netProfit;
            netProfit += (totalSeats_ / 4) * 150;
            netProfit -= venueCost_;
            return netProfit;
        }

        void takeInput()
        {
            std::cin >> startTime_ >> endTime_ >> baseTicketPrice_ >> totalSeats_ >> venueCost_;
        }

    private:
        int baseTicketPrice_;
        int totalSeats_;
        int venueCost_;
};

class Wedding : public Event
{
    public:
        Wedding(int sTime, int eTime, int baseAmount, int decorationCost, int guestCount, int venueCost) : Event(sTime, eTime)
        {
            baseAmount_ = baseAmount;
            decorationCost_ = decorationCost;
            guestCount_ = guestCount;
            venueCost_ = venueCost;
        }
        Wedding() : Event() {}

        double calculateProfit()
        {
            double cost = (guestCount_ > 200 ? venueCost_ * 3 : venueCost_);
            cost += (guestCount_ > 100 ? guestCount_ * 70 : guestCount_ * 100);
            cost += decorationCost_;
            return (double)baseAmount_ - cost;
        }

        void takeInput()
        {
            std::cin >> startTime_ >> endTime_ >> baseAmount_ >> decorationCost_ >> guestCount_ >> venueCost_;
        }
    
    private:
        int baseAmount_;
        int decorationCost_;
        int guestCount_;
        int venueCost_;
};

class Comparator
{
    public:
        Comparator() {}

        bool operator()(Event* a, Event* b)
        {
            if (a->endTime_ != b->endTime_) return a->endTime_ < b->endTime_;
            return a->startTime_ < b->startTime_; 
        }
};

class lowerBoundComparator
{
    public:
        lowerBoundComparator() {}

        bool operator()(Event* a, int time)
        {
            return a->endTime_ <= time;
        }
};

class EventScheduler
{
    public:
        EventScheduler(std::vector<Event*>& events) : events_(events) {}
        EventScheduler() {}

        double maxProfit()
        {
            int n = events_.size();
            std::vector<double> dp(n + 1);
            std::sort(events_.begin(), events_.end(), Comparator());

            for (int i = 0; i < n; i++)
            {
                int index = std::lower_bound(events_.begin(), events_.end(), events_[i]->startTime_, lowerBoundComparator()) - events_.begin();
                dp[i + 1] = std::max(dp[i], dp[index] + events_[i]->calculateProfit());
            }

            return dp[n];
        }

    private:
        std::vector<Event*> events_;
};

int main()
{
    int n;
    std::cin >> n;
    std::vector<Event*> events;
    events.reserve(n);

    int type;
    for (int i = 0; i < n; i++)
    {
        std::cin >> type;

        if (type == 1)
        {
            Concert* newConcert = new Concert();
            events.push_back(newConcert);
            std::cin >> *events.back();
        }
        else if (type == 2)
        {
            TheatreShow* newtheatreShow = new TheatreShow();
            events.push_back(newtheatreShow);
            std::cin >> *events.back();
        }
        else 
        {
            Wedding* newWedding = new Wedding();
            events.push_back(newWedding);
            std::cin >> *events.back();
        }
    }

    EventScheduler scheduler(events);
    std::cout << std::fixed << std::setprecision(2) << scheduler.maxProfit() << std::endl;

    for (auto& x : events)
    {
        free(x);
    }
}