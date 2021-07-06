
class T_Hist
{

public:
    T_Hist();
    T_Hist(unsigned char);
    T_Hist(std::vector<unsigned char> &);
    SO6 reconstruct(); // Gets an SO6 object by multiplying together the right and left hist
    std::vector<Z2> reconstruct_col(std::pair<SO6*, SO6*> &,char &) const; // Gets an SO6 object by multiplying together the right and left hist
    std::vector<Z2> reconstruct_row(char &) const; // Gets an SO6 object by multiplying together the right and left hist
    Z2 reconstruct_element(std::pair<SO6*,SO6*> &,char &, char &) const;
    const std::pair<SO6*,SO6*> get_factors() const;
    T_Hist operator*(T_Hist &); // Multiplication is history concatenation
    bool operator==(T_Hist &);
    bool operator==(T_Hist &) const;
    // bool operator<(T_Hist &);
    bool operator<(const T_Hist &) const;
    friend std::ostream &operator<<(std::ostream &, const T_Hist &);

    void set_lex_perm() ;
    std::vector<unsigned char> getHistory() { return hist; };
    void histInsert(unsigned char, unsigned char); // Currently unused

    static void make_T_matrices();
    static SO6 tsv[16];

    struct Node;
    static Node *head; // Head of the tree for looking up SO6 objects for reconstruct()
    static void initHead(); // Initalizes the head, called at the start of main()
    static void tableInsert(Node *, Node *, unsigned char); // Populates the so6 tree
    static void tableDelete(Node *, Node *); // Frees all memory allocated in the so6 tree
    static SO6* tableLookup(std::vector<unsigned char> &); // Get the matrix corresponding to a history vector
    static SO6 *curr;
    static T_Hist *curr_history;

private:
    std::vector<unsigned char> hist;
    std::vector<char> perm;    
};