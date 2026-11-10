#include <iostream>
#include <vector>
#include <string>
using namespace std;

// =========================================================
//  StarSystem  – one node in the M-ary tree
// =========================================================
class StarSystem {
public:
    int    system_id;
    string name;
    int    num_planets;
    double x, y, z;
    bool   is_habitable;

    vector<StarSystem*> children;    // up to M children
    vector<float>       travel_cost; // cost from parent → this child

    StarSystem(int id, string n, int planets,
               double x_, double y_, double z_, bool hab)
        : system_id(id), name(n), num_planets(planets),
          x(x_), y(y_), z(z_), is_habitable(hab) {}
};

// =========================================================
//  TravelRoute  – represents an edge (parent → child)
// =========================================================
class TravelRoute {
public:
    StarSystem* from;
    StarSystem* to;
    float       travel_cost;

    TravelRoute(StarSystem* f, StarSystem* t, float c)
        : from(f), to(t), travel_cost(c) {}
};

// =========================================================
//  Galaxy  – manages the M-ary tree
// =========================================================
class Galaxy {
public:
    int         M;      // max children per node
    StarSystem* root;

    // Overloaded constructors
    Galaxy() : M(3), root(nullptr) {}
    Galaxy(int m) : M(m), root(nullptr) {}
    Galaxy(int m, StarSystem* r) : M(m), root(r) {}

    // ---------- Attach a child with an edge cost ----------
    void addChild(StarSystem* parent, StarSystem* child, float cost) {
        if (!parent) return;
        if ((int)parent->children.size() >= M) {
            cout << "  [skipped] '" << parent->name
                 << "' already has M=" << M << " children\n";
            return;
        }
        parent->children.push_back(child);
        parent->travel_cost.push_back(cost);
    }

    // ---------- Helper: subtree size (nodes count) ----------
    int subtreeSize(StarSystem* node) const {
        if (!node) return 0;
        int total = 1;
        for (size_t i = 0; i < node->children.size(); i++)
            total += subtreeSize(node->children[i]);
        return total;
    }

    // ---------- (b) is_balanced ----------
    bool is_balanced(StarSystem* x) const {
        if (!x) return true;

        if (!x->children.empty()) {
            int mn = 1000000000, mx = -1;
            for (size_t i = 0; i < x->children.size(); i++) {
                int s = subtreeSize(x->children[i]);
                if (s < mn) mn = s;
                if (s > mx) mx = s;
            }
            if (mx - mn > 1) return false;
        }

        for (size_t i = 0; i < x->children.size(); i++)
            if (!is_balanced(x->children[i])) return false;

        return true;
    }

    // ---------- (c) count_leaves ----------
    int count_leaves(StarSystem* x) const {
        if (!x) return 0;
        if (x->children.empty()) return 1;
        int total = 0;
        for (size_t i = 0; i < x->children.size(); i++)
            total += count_leaves(x->children[i]);
        return total;
    }

    // ---------- Debug helper: print the tree ----------
    void printTree(StarSystem* x, int depth) const {
        if (!x) return;
        for (int i = 0; i < depth; i++) cout << "    ";
        cout << "- " << x->name
             << " (id=" << x->system_id
             << ", planets=" << x->num_planets
             << ", habitable=" << (x->is_habitable ? "yes" : "no")
             << ")\n";
        for (size_t i = 0; i < x->children.size(); i++) {
            for (int k = 0; k < depth + 1; k++) cout << "    ";
            cout << "|--[cost=" << x->travel_cost[i] << "]-->\n";
            printTree(x->children[i], depth + 1);
        }
    }
};

// =========================================================
//  Demo
// =========================================================
int main() {
    cout << "============ Galaxy: Weighted M-ary Tree ============\n\n";

    // --- Create star systems ---
    StarSystem* sun     = new StarSystem(1, "Sun",     8,  0.0,  0.0,  0.0, false);
    StarSystem* alpha   = new StarSystem(2, "Alpha",   3,  1.0,  1.0,  1.0, true);
    StarSystem* beta    = new StarSystem(3, "Beta",    2,  2.0, -1.0,  2.0, false);
    StarSystem* gamma   = new StarSystem(4, "Gamma",   1, -1.0,  2.0,  0.0, true);
    StarSystem* delta   = new StarSystem(5, "Delta",   5,  3.0,  3.0, -2.0, true);
    StarSystem* epsilon = new StarSystem(6, "Epsilon", 0,  4.0,  4.0,  4.0, false);
    StarSystem* zeta    = new StarSystem(7, "Zeta",    2, -2.0, -2.0, -2.0, false);

    // --- Build the galaxy: M = 3 ---
    Galaxy galaxy(3, sun);
    galaxy.addChild(sun,   alpha,   1.5f);
    galaxy.addChild(sun,   beta,    2.0f);
    galaxy.addChild(sun,   gamma,   3.2f);
    galaxy.addChild(alpha, delta,   0.8f);
    galaxy.addChild(alpha, epsilon, 1.2f);
    galaxy.addChild(beta,  zeta,    2.5f);

    // --- Print the tree ---
    cout << "Tree structure (M = " << galaxy.M << "):\n";
    galaxy.printTree(galaxy.root, 0);
    cout << "\n";

    // --- (b) Balanced? ---
    cout << "Is the tree balanced? "
         << (galaxy.is_balanced(galaxy.root) ? "YES" : "NO") << "\n";

    // --- (c) Leaf count ---
    cout << "Number of leaves: "
         << galaxy.count_leaves(galaxy.root) << "\n\n";

    // --- Extra: subtree sizes ---
    cout << "Subtree sizes:\n";
    cout << "  Sun     -> " << galaxy.subtreeSize(sun)     << "\n";
    cout << "  Alpha   -> " << galaxy.subtreeSize(alpha)   << "\n";
    cout << "  Beta    -> " << galaxy.subtreeSize(beta)    << "\n";
    cout << "  Gamma   -> " << galaxy.subtreeSize(gamma)   << "\n";
    cout << "  Delta   -> " << galaxy.subtreeSize(delta)   << "\n";
    cout << "  Epsilon -> " << galaxy.subtreeSize(epsilon) << "\n";
    cout << "  Zeta    -> " << galaxy.subtreeSize(zeta)    << "\n";

    return 0;
}