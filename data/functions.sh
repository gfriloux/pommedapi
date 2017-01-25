#!/usr/bin/env bash

jq_object_value_get()
{
  cat | jq -r "${1}"
}

jq_assert_val()
{
  val=$(jq_object_value_get "${2}" <${1})

  if [ "x${val}" != "x${3}" ]; then
    echo "Expected ${2} to be : ${3}"
    echo "Found : ${val}"
    exit 1
  fi
  echo "Checking ${2} : OK"
}
