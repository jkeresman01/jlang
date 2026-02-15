fn main() -> i32 {
    val v := std::Vector<i32>();
    v.push(10);
    v.push(20);
    v.push(30);
    printf("Size: %d\n", v.size());
    printf("Capacity: %d\n", v.capacity());
    printf("Element 0: %d\n", v.at(0));
    printf("Element 1: %d\n", v[1]);
    v.reserve(100);
    printf("After reserve - Capacity: %d\n", v.capacity());
    val popped := v.pop();
    printf("Popped: %d\n", popped);
    printf("Size after pop: %d\n", v.size());
    v.clear();
    printf("Size after clear: %d\n", v.size());
    v.free();
    return 0;
}
