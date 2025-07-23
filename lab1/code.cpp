#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

class Book
{
    private:
        std::string isbn_;
        int copiesAvailable_;
        int totalCopies_;
    
    public:
        std::string title_;
        std::string author_;

        Book(std::string title = "UnknownTitle", std::string author = "UnknownAuthor", std::string isbn = "ISBN", int copiesAvailable = 0, int totalCopies = 5) : title_(title), author_(author), isbn_(isbn), copiesAvailable_(copiesAvailable), totalCopies_(totalCopies) {}

        Book(const Book& oldBook, std::string newIsbn) 
        {
            isbn_ = newIsbn;
            copiesAvailable_ = oldBook.getCopiesAvailable();
            totalCopies_ = oldBook.getTotalCopies();
            title_ = oldBook.title_;
            author_ = oldBook.author_;
        }

        std::string getIsbn() 
        {
            return isbn_;
        }

        int getCopiesAvailable() const
        {
            return copiesAvailable_;
        }

        int getTotalCopies() const
        {
            return totalCopies_;
        }

        void updateCopies(int count)
        {
            if (copiesAvailable_ + count < 0)
            {
                std::cout << "Invalid request! Count becomes negative" << std::endl;
                return;
            }
            copiesAvailable_ += count;
            totalCopies_ += count;
        }

        bool borrowBook() 
        {
            copiesAvailable_--;
            return true;
        }

        bool returnBook()
        {
            copiesAvailable_++;
            return true;
        }

        void printDetails()
        {
            std::cout << title_ << " "  << author_ << std::endl;
        }

        bool operator==(const Book& other) const
        {
            return other.isbn_ == isbn_;
        }
};

class Member
{
    private:
        std::string memberID_;
        std::map<std::string,int> borrowedBooks;
        int borrowCount_;
        int borrowLimit_;
    
    public:
        std::string name_;

        Member(std::string memberID, std::string name, int borrowLimit = 3) : memberID_(memberID), borrowLimit_(borrowLimit), name_(name), borrowCount_(0) {}

        bool borrowBook(std::string isbn) 
        {
            if (borrowCount_ == borrowLimit_) 
            {
                std::cout << "Invalid request! Borrow limit exceeded\n";
                return false;
            }
            borrowCount_++;
            borrowedBooks[isbn]++;
            return true;
        }

        bool returnBook(std::string isbn)
        {
            if (borrowedBooks.find(isbn) == borrowedBooks.end()) 
            {
                std::cout << "Invalid request! Book not borrowed\n";
                return false;
            }
            borrowCount_--;
            borrowedBooks[isbn]--;
            if (borrowedBooks[isbn] == 0)
            {
                borrowedBooks.erase(isbn);
            }
            return true;
        }

        void printDetails()
        {
            for (auto& [isbn, noOfBorrowedCopies] : borrowedBooks)
            {
                std::cout << memberID_ << " " << name_ << " " << isbn << " " << noOfBorrowedCopies << std::endl;
            }
        }

        bool operator==(const Member& other) const
        {
            return memberID_ == other.memberID_;
        }

        friend class Library;
};

class Library
{
    private:
        std::vector<Book> books;
        std::vector<Member> members;

    public:
        bool addBook(Book& newBook)
        {
            if (std::find(books.begin(), books.end(), newBook) != books.end()) 
            {
                std::cout << "Invalid request! Book with same isbn already exists" << std::endl;
                return false;
            }
            books.push_back(newBook);
            return true;
        }

        bool addBook(std::string oldIsbn, std::string newIsbn)
        {
            int index = 0;
            for (auto &x : books)
            {
                if (x.getIsbn() == oldIsbn)
                {
                    break;
                }
                index++;
            }

            if (index == books.size()) return false;
            Book newBook(books[index], newIsbn);
            if (std::find(books.begin(), books.end(), newBook) != books.end()) 
            {
                std::cout << "Invalid request! Book with same isbn already exists" << std::endl;
                return false;
            }
            books.push_back(newBook);
            return true;
        }

        bool registerMember(const Member& newMember) 
        {
            if (std::find(members.begin(), members.end(), newMember) != members.end())
            {
                std::cout << "Invalid request! Member with same id already exists" << std::endl;
                return false;
            }
            members.push_back(newMember);
            return true;
        }

        void updateCopiesCount(std::string isbn, int newCount)
        {
            int index = 0;
            for (auto &x : books)
            {
                if (x.getIsbn() == isbn)
                {
                    break;
                }
                index++;
            }

            if (index == books.size()) return;
            books[index].updateCopies(newCount);           
        }
        bool borrowBook(std::string memberID, std::string isbn) 
        {
            int index = 0;
            for(auto& x : books)
            {
                if(x.getIsbn() == isbn)
                {
                    break; 
                }
                index++;
            }
            if (index == books.size()) return false;
            if (books[index].getCopiesAvailable() == 0)
            {
                std::cout << "Invalid request! Copy of book not available" << std::endl;
                return false;
            }
            int index2 = 0;
            for (auto& x : members)
            {
                if (x.memberID_ == memberID)
                {
                    break;
                }
                index2++;
            }
            if (index2 == members.size()) return false;
            
            if (members[index2].borrowBook(isbn))
            {
                books[index].borrowBook();
                return true;
            }
            else return false;
        }

        bool returnBook(std::string memberID, std::string isbn)
        {
            int index = 0;
            for(auto& x : books)
            {
                if(x.getIsbn() == isbn)
                {
                    break; 
                }
                index++;
            }
            if (index == books.size()) return false;
            if (books[index].getCopiesAvailable() == books[index].getTotalCopies())
            {
                std::cout << "Invalid request! Copy of book exceeds total copies" << std::endl;
                return false;
            }
            int index2 = 0;
            for (auto& x : members)
            {
                if (x.memberID_ == memberID)
                {
                    break;
                }
                index2++;
            }
            if (index2 == members.size()) return false;
            
            if (members[index2].returnBook(isbn))
            {
                books[index].returnBook();
                return true;
            }
            else return false;
        }

        void printLibraryDetails()
        {
            for (auto& x: books)
            {
                std::cout << x.title_ << " " << x.author_ << " " << x.getCopiesAvailable() << std::endl;
            }

            for (auto &x : members)
            {
                std::cout << x.memberID_ << " " << x.name_ << std::endl;
            }
        }

        void printBookDetails(std::string isbn)
        {
            int index = 0;
            for(auto& x : books)
            {
                if(x.getIsbn() == isbn)
                {
                    break; 
                }
                index++;
            }
            
            if (index == books.size()) return;
            books[index].printDetails();
        }

        void printMemberDetails(std::string memberID)
        {   
            int index = 0;
            for(auto &x : members)
            {
                if (x.memberID_ == memberID)
                {
                    break;
                }
                index++;
            }

            if (index == members.size()) return;
            members[index].printDetails();
        }
};

int main()
{
    std::string s;
    Library lib;
    while(true)
    {
        std::cin >> s;
        if (s == "Book")
        {
            std::string type;
            std::cin >> type;
            if (type == "None")
            {
                Book newBook;
                lib.addBook(newBook);
            }
            else if (type == "ExistingBook")
            {
                std::string oldIsbn, newIsbn;
                std::cin >> oldIsbn >> newIsbn;
                lib.addBook(oldIsbn, newIsbn);
            }
            else
            {
                std::string author, isbn;
                int copiesAvailable, totalCopies;
                std::cin >> author >> isbn >> copiesAvailable >> totalCopies;
                Book newBook(type, author, isbn, copiesAvailable, totalCopies);
                lib.addBook(newBook);
            }
        }
        else if (s == "UpdateCopiesCount")
        {
            std::string isbn;
            int newCount;
            std::cin >> isbn >> newCount;
            lib.updateCopiesCount(isbn, newCount);
        }
        else if (s == "Member")
        {
            std::string type;
            std::cin >> type;
            if (type == "NoBorrowLimit")
            {
                std::string memberID, name;
                std::cin >> memberID >> name;
                Member newMember(memberID, name);
                lib.registerMember(newMember);
            }
            else
            {
                std::string name;
                int borrowLimit;
                std::cin >> name >> borrowLimit;
                Member newMember(type, name, borrowLimit);
                lib.registerMember(newMember);
            }
        }
        else if (s == "Borrow")
        {
            std::string memberID, isbn;
            std::cin >> memberID >> isbn;
            lib.borrowBook(memberID, isbn);
        }
        else if (s == "Return")
        {
            std::string memberID, isbn;
            std::cin >> memberID >> isbn;
            lib.returnBook(memberID, isbn);
        }
        else if (s == "PrintBook")
        {
            std::string isbn;
            std::cin >> isbn;
            lib.printBookDetails(isbn);
        }
        else if (s == "PrintMember")
        {
            std::string memberID;
            std::cin >> memberID;
            lib.printMemberDetails(memberID);
        }
        else if (s == "PrintLibrary")
        {
            lib.printLibraryDetails();
        }
        else break;
    }
}