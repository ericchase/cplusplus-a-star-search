#include "stdafx.h"
#include "a-star.h"

class graph
{
public:

    // data
    static const int rows = 3;
    static const int cols = 3;

    class node
    {
    public:

        // data
        int index_of[rows * cols] = {};

        // constructors
        node ()
        {
            for (int i = 0; i < rows * cols; ++i)
            {
                index_of[i] = i;
            }
        }

        explicit node (const int values...)
        {
            va_list args;
            va_start(args, values);

            index_of[values] = 0;
            for (int i = 1; i < rows * cols; ++i)
            {
                index_of[va_arg(args, int)] = i;
            }

            va_end(args);
        }

        node (const node& original)
        {
            for (int i = 0; i < rows * cols; ++i)
            {
                index_of[i] = original.index_of[i];
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
            std::swap(n.index_of[0], n.index_of[n.index_of[0] - 1]);
            return n;
        }

        node move_up () const
        {
            node n(*this);
            std::swap(n.index_of[0], n.index_of[n.index_of[0] - cols]);
            return n;
        }

        node move_right () const
        {
            node n(*this);
            std::swap(n.index_of[0], n.index_of[n.index_of[0] + 1]);
            return n;
        }

        node move_down () const
        {
            node n(*this);
            std::swap(n.index_of[0], n.index_of[n.index_of[0] + cols]);
            return n;
        }

        std::queue <node> shuffle (const int shuffle_min, const int shuffle_max)
        {
            std::queue <node> shuffle_boards;
            shuffle_boards.push(*this);

            const int count = rand() % shuffle_min + (shuffle_max - shuffle_min);
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
            out << "|" << std::setw(len) << n.index_of[r * graph::cols + c];
        }
        out << "|" << "\n";
    }
    out << "+-----+" << "\n" << std::endl;
    return out.str();
}

int test_8puzzle ()
{
    graph puzzle;
    graph::node start, goal;

#if 0
    const size_t shuffle_count = shuffled_to_base(start, goal) - 1;
#else
    start = graph::node(5, 4, 6, 3, 1, 8, 2, 0, 7);
    //start = graph::node(1, 2, 3, 0, 4, 7, 5, 6, 8);
    const size_t shuffle_count = 0;
#endif

    std::stack <graph::node> optimal_path = a_star <graph>(puzzle, start, goal);
    const size_t solution_count = optimal_path.size();

    std::cout << "Shuffle count: " << shuffle_count << " moves." << std::endl;
    std::cout << "Solution count (one): " << solution_count << " moves." << std::endl;
    std::cout << std::endl;

    //std::stack <graph::node> optimal_path_two
    //        = a_star_two <graph::node, int>(
    //            start, goal,
    //            graph::node::getNeighbors,
    //            graph::node::getCost,
    //            graph::node::getEstimate
    //        );

    //const size_t solution_count_two = optimal_path_two.size();

    //const size_t max = std::max(solution_count_one, solution_count_two);
    //for (size_t a = 0; a < max; ++a)
    //{
    //    static graph::node board_a, board_b;

    //    if (!optimal_path_one.empty())
    //    {
    //        board_a = optimal_path_one.top();
    //        optimal_path_one.pop();
    //    }
    //    else board_a = graph::node(0, 0, 0, 0, 0, 0, 0, 0, 0);

    //    if (!optimal_path_two.empty())
    //    {
    //        board_b = optimal_path_two.top();
    //        optimal_path_two.pop();
    //    }
    //    else board_b = graph::node(0, 0, 0, 0, 0, 0, 0, 0, 0);

    //    std::cout << Puzzle::print(board_a, board_b);
    //}

    //std::cout << "Solution count (two): " << solution_count_two << " moves." << std::endl;

    return 0;
}

int main ()
{
    return 0;
}
