struct pageInfo {
  pte_t *pte;
  uint off;
  uint count; // refCount for NFU, time for FIFO
};
