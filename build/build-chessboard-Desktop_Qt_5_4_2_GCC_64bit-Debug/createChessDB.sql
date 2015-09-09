-- phpMyAdmin SQL Dump
-- version 4.0.10deb1
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Sep 05, 2015 at 12:17 PM
-- Server version: 5.5.43-0ubuntu0.14.04.1
-- PHP Version: 5.5.9-1ubuntu4.11

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Database: `mychessDB`
--
CREATE DATABASE IF NOT EXISTS `mychessDB` DEFAULT CHARACTER SET latin1 COLLATE latin1_swedish_ci;
USE `mychessDB`;

-- --------------------------------------------------------

--
-- Table structure for table `event`
--

DROP TABLE IF EXISTS `event`;
CREATE TABLE IF NOT EXISTS `event` (
  `event_id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(200) NOT NULL,
  `site` varchar(200) NOT NULL,
  `date` varchar(50) NOT NULL,
  PRIMARY KEY (`event_id`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=283 ;

-- --------------------------------------------------------

--
-- Table structure for table `game`
--

DROP TABLE IF EXISTS `game`;
CREATE TABLE IF NOT EXISTS `game` (
  `game_id` int(11) NOT NULL AUTO_INCREMENT,
  `moves` varchar(2000) NOT NULL,
  `white_id` int(11) NOT NULL,
  `black_id` int(11) NOT NULL,
  `event_id` int(11) NOT NULL,
  `round` int(11) NOT NULL,
  `result` varchar(7) NOT NULL,
  `white_elo` int(11) NOT NULL,
  `black_elo` int(11) NOT NULL,
  `eco` varchar(4) NOT NULL,
  `positions` varchar(3000) NOT NULL,
  `registration_time` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00' ON UPDATE CURRENT_TIMESTAMP,
  `date` varchar(26) NOT NULL,
  PRIMARY KEY (`game_id`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=5360 ;

-- --------------------------------------------------------

--
-- Table structure for table `player`
--

DROP TABLE IF EXISTS `player`;
CREATE TABLE IF NOT EXISTS `player` (
  `player_id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(100) NOT NULL,
  `fide_id` int(11) NOT NULL,
  `elo` int(11) NOT NULL,
  `fics_handle` varchar(17) NOT NULL,
  `last_fics_scan` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`player_id`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=2491 ;

-- --------------------------------------------------------

--
-- Table structure for table `position`
--

DROP TABLE IF EXISTS `position`;
CREATE TABLE IF NOT EXISTS `position` (
  `position_id` int(11) NOT NULL AUTO_INCREMENT,
  `fen1` varchar(8) NOT NULL,
  `fen2` varchar(8) NOT NULL,
  `fen3` varchar(8) NOT NULL,
  `fen4` varchar(8) NOT NULL,
  `fen5` varchar(8) NOT NULL,
  `fen6` varchar(8) NOT NULL,
  `fen7` varchar(8) NOT NULL,
  `fen8` varchar(8) NOT NULL,
  `active_color` char(1) NOT NULL,
  `castle` char(4) NOT NULL DEFAULT '----',
  `en_passant` varchar(2) NOT NULL DEFAULT '-',
  `parent` int(11) NOT NULL,
  `game_id` int(11) NOT NULL,
  PRIMARY KEY (`position_id`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=359399 ;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
