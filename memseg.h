#pragma once

enum segment_status {
IS_FREE, // отрезок свободен, в нём можно выделять память
IS_ALLOCATED, // отрезок занят
IS_DELETED, // отрезок невалиден, его нужно удалить вместе со всеми указателями на него (деталь реализации)
};

struct memory_segment {
    segment_status status;
    int length;

    memory_segment(int status, int length) {
        this->status = (segment_status)status;
        this->length = length;
    }
};