#include "stdafx.h"
#include "a-star.h"

class graph
{
public:

    // data
    static const int rows = 3;
    static const int cols = 3;
    static int base[rows * cols];

    class node
    {
    public:

        // data
        int index_of[rows * cols] = {};
        int value_at[rows * cols] = {};

        // constructors
        node ()
        {
            for (int i = 0; i < rows * cols; ++i)
            {
                value_at[i] = base[i];
                index_of[base[i]] = i;
            }
        }

        explicit node (const int values...)
        {
            va_list args;
            va_start(args, values);

            index_of[values] = 0;
            value_at[0] = values;
            for (int i = 1; i < rows * cols; ++i)
            {
                const int n = va_arg(args, int);
                index_of[n] = i;
                value_at[i] = n;
            }

            va_end(args);
        }

        node (const node& original)
        {
            for (int i = 0; i < rows * cols; ++i)
            {
                index_of[i] = original.index_of[i];
                value_at[i] = original.value_at[i];
            }
        }

        // comparator methods

        bool operator== (const node& other) const
        {
            for (int i = 0; i < rows * cols; ++i)
            {
                if (index_of[i] != other.index_of[i])
                    return false;
            }
            return true;
        }

        bool operator!= (const node& other) const
        {
            return !operator==(other);
        }

        bool operator< (const node& other) const
        {
            for (int i = 0; i < rows * cols; ++i)
            {
                if (index_of[i] < other.index_of[i])
                    return true;
                if (index_of[i] > other.index_of[i])
                    return false;
            }
            return false;
        }

        // other methods

        bool can_move_left () const
        {
            return !(index_of[0] % cols == 0);
        }

        bool can_move_up () const
        {
            return !(index_of[0] < cols);
        }

        bool can_move_right () const
        {
            return !(index_of[0] % cols == cols - 1);
        }

        bool can_move_down () const
        {
            return !(index_of[0] > cols * rows - cols - 1);
        }

        node move_left () const
        {
            node n(*this);
            n.index_of[0] = index_of[0] - 1; // move index of 0 left
            n.index_of[value_at[index_of[0] - 1]] = index_of[0]; // move index of value right
            std::swap(n.value_at[index_of[0]], n.value_at[index_of[0] - 1]); // swap values at indices
            return n;
        }

        node move_up () const
        {
            node n(*this);
            n.index_of[0] = index_of[0] - cols; // move index of 0 up
            n.index_of[value_at[index_of[0] - cols]] = index_of[0]; // move index of value down
            std::swap(n.value_at[index_of[0]], n.value_at[index_of[0] - cols]); // swap values at indices
            return n;
        }

        node move_right () const
        {
            node n(*this);
            n.index_of[0] = index_of[0] + 1; // move index of 0 right
            n.index_of[value_at[index_of[0] + 1]] = index_of[0]; // move index of value left
            std::swap(n.value_at[index_of[0]], n.value_at[index_of[0] + 1]); // swap values at indices
            return n;
        }

        node move_down () const
        {
            node n(*this);
            n.index_of[0] = index_of[0] + cols; // move index of 0 up
            n.index_of[value_at[index_of[0] + cols]] = index_of[0]; // move index of value down
            std::swap(n.value_at[index_of[0]], n.value_at[index_of[0] + cols]); // swap values at indices
            return n;
        }

        std::queue <node> shuffle (const int shuffle_min, const int shuffle_max)
        {
            std::queue <node> shuffle_boards;
            shuffle_boards.push(*this);

            const int count = rand() % (shuffle_max - shuffle_min + 1) + shuffle_min;
            for (int i = 0; i < count; ++i)
            {
                std::vector <int> allowed_moves;
                if (can_move_left()) allowed_moves.push_back(0);
                if (can_move_up()) allowed_moves.push_back(1);
                if (can_move_right()) allowed_moves.push_back(2);
                if (can_move_down()) allowed_moves.push_back(3);

                switch (allowed_moves.at(rand() % allowed_moves.size()))
                {
                    case 0:
                        *this = move_left();
                        break;
                    case 1:
                        *this = move_up();
                        break;
                    case 2:
                        *this = move_right();
                        break;
                    case 3:
                        *this = move_down();
                        break;
                    default:
                        assert(false);
                }
                shuffle_boards.push(*this);
            }

            return shuffle_boards;
        }
    };

    typedef int weight;

    // constructors

    explicit graph ()
    {
        for (int i = 0; i < rows * cols; ++i)
        {
            base[i] = i;
        }
    }

    explicit graph (const int values...)
    {
        va_list args;
        va_start(args, values);

        base[0] = values;
        for (int i = 1; i < rows * cols; ++i)
        {
            const int n = va_arg(args, int);
            base[i] = n;
        }

        va_end(args);
    }

    // a-star required methods

    static std::vector <node> get_neighbors_of (const node& a)
    {
        std::vector <node> neighbors;

        if (a.can_move_left()) neighbors.push_back(a.move_left());
        if (a.can_move_up()) neighbors.push_back(a.move_up());
        if (a.can_move_right()) neighbors.push_back(a.move_right());
        if (a.can_move_down()) neighbors.push_back(a.move_down());

        return neighbors;
    }

    static weight get_cost_between (const node&, const node&)
    {
        return 1;
    }

    weight get_estimate_between (const node& a, const node& b) const
    {
        weight estimate = 0;

        for (int i = 0; i < rows * cols; ++i)
        {
            estimate += manhattan_distance(a.index_of[i], b.index_of[i]);
        }

        return estimate;
    }

    // other methods

    int manhattan_distance (const int a_i, const int b_i) const
    {
        return std::abs(a_i / rows - b_i / rows) + std::abs(a_i % cols - b_i % cols);
    }
};

int graph::base[rows * cols] = {};

std::string print_node (const graph::node& n)
{
    // convert grid size into string
    std::stringstream ss;
    ss << graph::rows * graph::cols;
    std::string s;
    ss >> s;

    // number of characters in the grid-size-string
    const size_t len = s.size();

    // not really made for grids with side size other than 3
    // but this can be easily implemented for other sizes by the user
    std::stringstream out;
    out << "+board+" << std::setw(len) << "\n";
    for (int r = 0; r < graph::rows; ++r)
    {
        for (int c = 0; c < graph::cols; ++c)
        {
            out << "|" << std::setw(len) << n.value_at[r * graph::cols + c];
        }
        out << "|" << "\n";
    }
    out << "+-----+" << "\n" << std::endl;
    return out.str();
}

size_t shuffled_to_base (graph::node& start)
{
    //std::cout << "Shuffling board." << "\n" << std::endl;
    std::queue <graph::node> shuffle_boards = start.shuffle(1000, 1000);
    const size_t shuffle_count = shuffle_boards.size() - 1;
    //while (!shuffle_boards.empty())
    //{
    //    std::cout << print_node(shuffle_boards.front());
    //    shuffle_boards.pop();
    //}
    //std::cout << "Shuffle done. Attempting to solve." << "\n" << std::endl;
    return shuffle_count;
}

int test_8puzzle ()
{
    std::iostream::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    //const graph puzzle;
    const graph puzzle(1, 2, 3, 4, 5, 6, 7, 8, 0);
    graph::node start, goal;

#if 1
    const size_t shuffle_count = shuffled_to_base(start);
#else
    //start = graph::node(8, 6, 7, 2, 5, 4, 3, 0, 1);
    //start = graph::node(6, 4, 7, 8, 5, 0, 3, 2, 1);
    //goal = graph::node(1, 2, 3, 4, 5, 6, 7, 8, 0);

    //start = graph::node(5, 4, 6, 3, 1, 8, 2, 0, 7);
    //start = graph::node(1, 2, 3, 0, 4, 7, 5, 6, 8);
    const size_t shuffle_count = 0;
#endif

    std::stack <graph::node> optimal_path = a_star <graph>(puzzle, start, goal);
    const size_t solution_count = optimal_path.size() - 1;

    if (solution_count > 30)
    {
        std::cout << print_node(start);
        //while (!optimal_path.empty())
        //{
        //  std::cout << print_node(optimal_path.top());
        //  optimal_path.pop();
        //}
        std::cout << "Shuffle count: " << shuffle_count << " moves." << std::endl;
        std::cout << "Solution count: " << solution_count << " moves." << std::endl;
        std::cout << std::endl;
    }

    return 0;
}

int main (int argc, char** argv)
{
    std::srand(unsigned int(std::time(nullptr)));

    size_t count = 1;

    if (argc > 1)
    {
        std::stringstream ss;
        ss << argv[1];
        ss >> count;
    }

    for (size_t i = 0; i < count; ++i)
    {
        test_8puzzle();
    }

    return 0;
}
