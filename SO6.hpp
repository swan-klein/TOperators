
class SO6
{
public:
    SO6();
    // SO6(std::string); //initializes zero matrix
    // SO6(Z2[6][6], std::string); //initializes matrix according to a 6x6 array of Z2
    SO6(std::vector<int8_t> t);
    SO6(Z2[6][6], std::vector<int8_t> t); //initializes matrix according to a 6x6 array of Z2
    SO6 operator*(SO6 &);                 //mutliplication
    void fixSign();
    void lexOrder();
    void reduced_rep();
    inline Z2 &operator()(int8_t col, int8_t row) { return arr[col][row]; } //returns the (i,j)th entry
    // bool operator%(SO6&);
    bool operator<(const SO6 &) const;
    const Z2 &operator()(int8_t i, int8_t j) const { return arr[i][j]; } //returns the (i,j)th entry but for const
    bool operator==(SO6 &);                                              //checking equality up to signed permutation
    Z2 *operator[](const int8_t i) { return arr[i]; }                    // Return the array element needed.
    const Z2 *operator[](const int8_t i) const { return arr[i]; }        // Return the array element needed.
    // inline std::string getName(){return(name);} //getter for Name
    // inline void setName(std::string newName){name = newName;}
    int8_t getLDE() { return (0); } //getter for LDE
    SO6 transpose();
    // void genOneNorm(); //Returns 1-norm of the first row
    // bool isPerm(SO6 s); //Returns true if and only if s is similar to this object
    // inline float normFloat() { return norm.toFloat(); }
    int8_t genLDE();                                              // generates LDE, called after multiplication and constructor
    friend std::ostream &operator<<(std::ostream &, const SO6 &); //display
    bool operator==(const SO6 &other) const;
    SO6 residue(); // returns the residue matrix using Z2::pattern()
    inline int8_t getLast()
    {
        if (hist.size() != 0)
            return hist[0];
        return -1;
    };
    std::vector<int8_t> getHistory() { return hist; };
    
    static const SO6 tMatrix(int8_t i, int8_t j , int8_t matNum) {
        SO6 t = SO6::identity({matNum});                    // Initialize to the identity matrix
        t[i][i] = Z2::inverse_root2();                      // Change the i,j cycle to appropriate 1/sqrt(2)
        t[j][j] = Z2::inverse_root2();
        t[i][j] = Z2::inverse_root2();
        if (abs(i - j) != 1)  t[i][j].negate(); 
        t[j][i] = -t[i][j];
        return t;
    }; 

    static const SO6 identity(int8_t matNum) {
        SO6 t({matNum});
        for(int8_t k = 0; k < 6; k++) t[k][k]=1;
        return t;
    }

    static std::vector<bool> column_signs(SO6 &mat)
    {
        std::vector<bool> ret(6,0);
        for (int col = 0; col < 6; col++) {
            int row = 0;
            while(mat[col][row]==0) {
                row++;
            }
            bool tmp = (mat[col][row] < 0);
            ret[col] = (mat[col][row] < 0);
        }
        return ret;
    }

    static std::vector<int> lexicographic_permutation(SO6 &mat) {  
        std::vector<bool> signs = column_signs(mat);
        std::vector<int> index(6,0);
        for (int i = 0 ; i < 6 ; i++) index[i] = i;
        std::sort(index.begin(), index.end(),
            [&](const int& a, const int& b) {
                return SO6::lexLess(mat[a],mat[b],signs[a],signs[b]);
            }
        );
        for (int i=0; i<6; i++) {
            if(signs[index[i]]) {
                 index[i] = -index[i];
            }
        }
        return index;
    }

    /**
     * Method to compare two Z2 arrays of length 6 lexicographically
     * @param first array of Z2 of length 6
     * @param second array of Z2 of length 6
     * @return -1 if first < second, 0 if equal, 1 if first > second
     */
    static const bool lexLess(Z2 * first, Z2 * second) {
        for (int8_t i = 0; i < 6; i++)
        {
            if (first[i] != second[i]) return first[i] < second[i];
        }
        return false;
    }

    /**
     * Method to compare two Z2 arrays of length 6 lexicographically
     * @param first array of Z2 of length 6
     * @param second array of Z2 of length 6
     * @return -1 if first < second, 0 if equal, 1 if first > second
     */
    static bool lexLess(Z2 * first, Z2 * second, bool signA, bool signB) {
        for (int8_t i = 0; i < 6; i++) {
            Z2 f = first[i];
            Z2 s = second[i];

            if(signA) f.negate();
            if(signB) s.negate();
            if (f != s) return f < s;
        }
        return false;
    }

private:
    Z2 arr[6][6];
    std::vector<int8_t> hist;
};

