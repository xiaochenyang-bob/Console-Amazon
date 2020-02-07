#ifndef REVIEW_H
#define REVIEW_H
#include <string>
#include <iostream>

// NOTE: You may add any data members or functions you deem useful

struct Review
{
    // Review() :  rating(0), username(), date(), reviewText()
    // { }
    // Review(int rate,
    //        const std::string& u,
    //        const std::string& d,
    //        const std::string& review_text) :

    //     rating(rate),
    //     username(u),
    //     date(d),
    //     reviewText(review_text)
    // { }
    Review();
    Review(int rate, const std::string& u, const std::string& d, 
           const std::string& review_text);
    ~Review();
    int getRating() const;
    std::string getUsername() const;
    std::string getDate() const;
    std::string getText() const;
    virtual void dump(std::ostream& os) const;
    void viewReviews(std::ostream& os) const;

    int rating;
    std::string username;
    std::string date;
    std::string reviewText;
};

#endif
