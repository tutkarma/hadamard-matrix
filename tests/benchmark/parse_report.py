from argparse import ArgumentParser
import re
import xml.etree.ElementTree as ET

import matplotlib.pyplot as plt


parser = ArgumentParser()
parser.add_argument('-r', '--check-report-file',
                    dest='report',
                    default='test.xml',
                    help='Check report file')


def save_plot(x, y, title, filename):
    fig, ax = plt.subplots()
    ax.plot(x, y)

    ax.set(xlabel='order', ylabel='time (s)',
           title=title)
    ax.grid()

    fig.savefig(filename)


def extract_order(line):
    return int(re.search(r'h(\d+)', line.split(',')[0]).group(1))


def extract_tests(file, item):
    d = {}
    with open(file, 'r') as f:
        content = f.read()

    idx1 = content.find(item)
    idx2 = content.find(';', idx1)
    files = content[idx1:idx2].split('\n')

    i = 0
    for line in files:
        if 'data' in line and '//' not in line:
            d[i] = extract_order(line)
            i += 1

    return d


def get_dict_iters():
    equal_iters = extract_tests('../test_equivalence.c', 'files_eq []')
    non_equal_iters = extract_tests('../test_equivalence.c', 'files_noneq []')
    return equal_iters, non_equal_iters


def main():
    arguments = parser.parse_args()

    equal_iters, non_equal_iters = get_dict_iters()
    root = ET.parse(arguments.report).getroot()

    eq_test_durations = []
    noneq_test_durations = []

    for suite in root.iter('{http://check.sourceforge.net/ns}suite'):
        title = suite.find('{http://check.sourceforge.net/ns}title').text
        if title == 'Equal Test Suite':
            for test in suite.iter('{http://check.sourceforge.net/ns}test'):
                duration = test.find('{http://check.sourceforge.net/ns}duration').text
                eq_test_durations.append(duration)
        elif title == 'Nonequal Test Suite':
            for test in suite.iter('{http://check.sourceforge.net/ns}test'):
                duration = test.find('{http://check.sourceforge.net/ns}duration').text
                noneq_test_durations.append(duration)

    save_plot(list(equal_iters.values()),
        eq_test_durations,
        'Equivalent matrices',
        'equal.png')

    save_plot(list(non_equal_iters.values()),
        noneq_test_durations,
        'Non-equivalent matrices',
        'nonequal.png')


if __name__ == '__main__':
    main()