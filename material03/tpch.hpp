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

    static Order fromFields(const std::vector<std::string>& row) {
        Order o;
        o.orderkey      = std::stoi(row[0]);
        o.custkey       = std::stoi(row[1]);
        o.orderstatus   = row[2][0];
        o.totalprice    = std::stod(row[3]);
        o.orderdate     = row[4];
        o.orderpriority = row[5];
        o.clerk         = row[6];
        o.shippriority  = std::stoi(row[7]);
        o.comment       = row[8];
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

    static Lineitem fromFields(const std::vector<std::string>& row) {
        Lineitem l;
        l.orderkey      = std::stoi(row[0]);
        l.partkey       = std::stoi(row[1]);
        l.suppkey       = std::stoi(row[2]);
        l.linenumber    = std::stoi(row[3]);
        l.quantity      = std::stod(row[4]);
        l.extendedprice = std::stod(row[5]);
        l.discount      = std::stod(row[6]);
        l.tax           = std::stod(row[7]);
        l.returnflag    = row[8][0];
        l.linestatus    = row[9][0];
        l.shipdate      = row[10];
        l.commitdate    = row[11];
        l.receiptdate   = row[12];
        l.shipinstruct  = row[13];
        l.shipmode      = row[14];
        l.comment       = row[15];
        return l;
    }
};
#endif //TPCH_JOIN_TPCH_HPP
