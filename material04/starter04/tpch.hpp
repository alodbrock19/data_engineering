#ifndef TPCH_JOIN_TPCH_HPP
#define TPCH_JOIN_TPCH_HPP

#include <string>
#include <vector>

// One row of orders.tbl
struct Order {
    int orderkey;
    int custkey;
    char orderstatus;
    double totalprice;
    std::string orderdate;   // kept as text, e.g. "1996-01-02"
    std::string orderpriority;
    std::string clerk;
    int shippriority;
    std::string comment;

    static Order fromFields(const std::vector<std::string>& f) {
        Order o;
        o.orderkey      = std::stoi(f[0]);
        o.custkey       = std::stoi(f[1]);
        o.orderstatus   = f[2][0];
        o.totalprice    = std::stod(f[3]);
        o.orderdate     = f[4];
        o.orderpriority = f[5];
        o.clerk         = f[6];
        o.shippriority  = std::stoi(f[7]);
        o.comment       = f[8];
        return o;
    }
};

// One row of lineitem.tbl
struct Lineitem {
    int orderkey;
    int partkey;
    int suppkey;
    int linenumber;
    double quantity;
    double extendedprice;
    double discount;         // e.g. 0.05 means 5%
    double tax;
    char returnflag;
    char linestatus;
    std::string shipdate;    // kept as text, e.g. "1996-03-13"
    std::string commitdate;
    std::string receiptdate;
    std::string shipinstruct;
    std::string shipmode;
    std::string comment;

    static Lineitem fromFields(const std::vector<std::string>& f) {
        Lineitem l;
        l.orderkey      = std::stoi(f[0]);
        l.partkey       = std::stoi(f[1]);
        l.suppkey       = std::stoi(f[2]);
        l.linenumber    = std::stoi(f[3]);
        l.quantity      = std::stod(f[4]);
        l.extendedprice = std::stod(f[5]);
        l.discount      = std::stod(f[6]);
        l.tax           = std::stod(f[7]);
        l.returnflag    = f[8][0];
        l.linestatus    = f[9][0];
        l.shipdate      = f[10];
        l.commitdate    = f[11];
        l.receiptdate   = f[12];
        l.shipinstruct  = f[13];
        l.shipmode      = f[14];
        l.comment       = f[15];
        return l;
    }
};
#endif //TPCH_JOIN_TPCH_HPP
