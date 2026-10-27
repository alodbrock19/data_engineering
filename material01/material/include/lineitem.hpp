#pragma once

struct LineItem {
    
    int l_orderkey;
    double l_extendedprice;
    double l_discount;
    char l_returnflag;
};

struct LineItem2 {
    
    double l_extendedprice;
    double l_discount;
    int l_orderkey;
    char l_returnflag;
};