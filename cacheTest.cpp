#include "cachehead.h"
#include <iostream>

int main(){
    RAM ram;
    Cache cache(ram);

    int hits = 0, misses = 0;
    auto access = [&](uint16_t addr) -> uint16_t {
        // peek tagMatch before access to count hit/miss
        uint8_t tag, index, offset;
        addsplitter(addr, tag, index, offset);
        int result = cache.setArray[index].tagMatch(tag);
        if (result == -1) misses++;
        else hits++;
        return cache.access(addr);
    };

    //Test 1: Sequential access (spatial locality)
    std::cout << "\n=== Test 1: Sequential Read (spatial locality) ===\n";
    hits = 0; misses = 0;
    for (int i = 0; i < 64; i++){
        uint16_t addr = i * 2; // halfword stride
        access(addr);
    }
    std::cout << "Accesses: 64 | Hits: " << hits << " | Misses: " << misses
              << " | Hit Rate: " << (hits * 100.0 / 64) << "%\n";

    // Test 2: Repeated access to same block (temporal locality)
    std::cout << "\n=== Test 2: Repeated Access (temporal locality) ===\n";
    hits = 0; misses = 0;
    for (int i = 0; i < 32; i++){
        access(0x0000); // same address 32 times
        access(0x0002);
        access(0x0004);
    }
    int total = 96;
    std::cout << "Accesses: " << total << " | Hits: " << hits << " | Misses: " << misses
              << " | Hit Rate: " << (hits * 100.0 / total) << "%\n";

    // ── Test 3: Write then read back ─────────────────────────────────────────
    std::cout << "\n=== Test 3: Write → Read Consistency ===\n";
    cache.write(0x0100, 0xABCD);
    uint16_t val = cache.access(0x0100);
    std::cout << "Wrote: 0xABCD | Read back: 0x" << std::hex << val
              << (val == 0xABCD ? " | PASS" : " | FAIL") << std::dec << "\n";

    // ── Test 4: Dirty writeback — fill set, force eviction ───────────────────
    std::cout << "\n=== Test 4: Dirty Eviction Writeback ===\n";
    // Write to two lines in set 0 (tag=0 and tag=1, index=0)
    cache.write(0x0000, 0x1234); // tag=0, index=0
    cache.write(0x0200, 0x5678); // tag=1, index=0
    // Force eviction of one — tag=2, index=0
    cache.write(0x0400, 0x9ABC); // tag=2, index=0 — triggers evict
    // Check RAM was written back for evicted dirty line
    uint16_t ramVal = ((uint16_t)ram.storage[0x0000] << 8) | ram.storage[0x0001];
    std::cout << "Evicted dirty line writeback to RAM[0x0000]: 0x" << std::hex << ramVal
              << (ramVal == 0x1234 ? " | PASS" : " | FAIL") << std::dec << "\n";

    // ── Test 5: LRU eviction order ────────────────────────────────────────────
    std::cout << "\n=== Test 5: LRU Policy ===\n";
    RAM ram2;
    Cache cache2(ram2);
    cache2.access(0x0000); // load tag=0 into set 0 → line0
    cache2.access(0x0200); // load tag=1 into set 0 → line1
    cache2.access(0x0000); // access tag=0 again → line0 is now MRU, line1 is LRU
    cache2.access(0x0400); // tag=2 → should evict line1 (LRU)
    // tag=1 should now be gone, tag=0 should still be present
    uint8_t t, i, o;
    addsplitter(0x0200, t, i, o);
    int r1 = cache2.setArray[i].tagMatch(t); // tag=1 — should be -1 (evicted)
    addsplitter(0x0000, t, i, o);
    int r0 = cache2.setArray[i].tagMatch(t); // tag=0 — should be 0 or 1 (still present)
    std::cout << "tag=1 evicted: " << (r1 == -1 ? "PASS" : "FAIL") << "\n";
    std::cout << "tag=0 retained: " << (r0 != -1 ? "PASS" : "FAIL") << "\n";

    std::cout << "\n=== All tests complete ===\n";
    return 0;
}