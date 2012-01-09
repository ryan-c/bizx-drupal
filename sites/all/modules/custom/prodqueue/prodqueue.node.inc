<?php

function _theme_prodqueue_current_state($value) {
	// Used to determine and return class name of not-started, in-progress, or done based on value.
	
	if($value == -1) {
		return 'not-started';
	}
	elseif($value == 0) {
		return 'in-progress';
	}
	else {
		return 'done';
	}
}

function theme_prodqueue_node_progress($node) {
	drupal_add_css(drupal_get_path('module', 'prodqueue') . '/css/prodqueue_node_edit.css');
	
	$output = '<div class="prodqueue_node_progress"><ol>';
	
	$output .= '<li>Production Queue: </li>';
	$output .= '<li class="done">1. New</li>';
	$output .= '<li class="'. _theme_prodqueue_current_state($node->prodqueue['content_ready']) .'">2. Content</li>';
	$output .= '<li class="'. _theme_prodqueue_current_state($node->prodqueue['photo_ready']) .'">3. Photo</li>';
	$output .= '<li class="'. _theme_prodqueue_current_state($node->prodqueue['editor_ready']) .'">4. Review</li>';
	$output .= '<li class="'. ($node->status == 1 ? "done" : "not-started") .'">5. Published</li>';
	
	$output .= '</ol></div><div class="prodqueue_node_info">';
	
	$output .= prodqueue_get_status($node);
	
	$output .= '</div>';
	
	return $output;
}

function prodqueue_get_status($node) {
	if($node->prodqueue['checked_out_uid'] !== 0) {
		// Currently checked out.
		$account = user_load($node->prodqueue['checked_out_uid']);
		return 'Content was checked-out by ' . l($account->name, 'user/' . $account->uid) . ' on '. date('M d, Y H:m A', $node->prodqueue['checked_out_time']) .'.';
	} else {
		return 'Content is not currently checked-out by anyone. It is probably in a queue waiting to be worked on.';
	}
}